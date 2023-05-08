#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "mpc.h"
#include "parser.h"

void _printAST(mpc_ast_t* node, int level) {
    for (int i = 0; i < level; i++) { printf("\t"); }
    printf("%s:%s\t-->(%d)\n", strcmp(node->tag, "") ? node->tag : "/",
           strcmp(node->contents, "") ? node->contents : "_",
           node->children_num);
    for (int i = 0; i < node->children_num; i++) {
        _printAST(node->children[i], level + 1);
    }
}
void printAST(mpc_ast_t* node) {
    _printAST(node, 0);
}

long evalOp(char* op, long x, int y) {
    if (!strcmp(op, "+")) { return x + y; }
    if (!strcmp(op, "-")) { return x - y; }
    if (!strcmp(op, "*")) { return x * y; }
    if (!strcmp(op, "/")) { return x / y; }
    if (!strcmp(op, "^")) {
        long acc = 1;
        for (int i = 0; i < y; i++) { acc *= x; }
        return acc;
    }
    if (!strcmp(op, "%")) { return x - y * (x / y); }
    if (!strcmp(op, "min")) { return x < y ? x : y; }
    if (!strcmp(op, "max")) { return x > y ? x : y; }
    assert(0 && "unreachable code reached in evalOp()");
}

typedef enum {
    NUM,
    EXPR,
    ROOT,
} SYMBOL;

SYMBOL parseTag(char* tag) {
    if (!strcmp(tag, "expr|number|regex")) { return NUM; }
    if (!strcmp(tag, "expr|>")) { return EXPR; }
    if (!strcmp(tag, ">")) { return ROOT; }
    assert(0 && "unreachable code reached in parseTag()");
}

long eval(mpc_ast_t* node) {
    switch (parseTag(node->tag)) {
    case NUM: return atoi(node->contents); break;

    case EXPR:;
        char* op = node->children[1]->contents;

        if (!strcmp(op, "-") && !(strcmp(node->children[3]->contents, ")"))) {
            return eval(node->children[2]) * -1;
        }

        long x = eval(node->children[2]);

        for (int i = 3; i < node->children_num - 1; i++) {
            long y = eval(node->children[i]);
            x      = evalOp(op, x, y);
        }
        return x;
        break;

    case ROOT: return eval(node->children[1]); break;
    default: assert(0 && "unreachable code reached in eval()"); break;
    }
    assert(0 && "unreachable code reached in eval()");
}

int main() {
    setup_parser();

    while (1) {
        mpc_result_t r;

        char* readLine = readline("> ");
        if (!readLine) { break; }
        add_history(readLine);

        if (mpc_parse("<stdin>", readLine, Program, &r)) {
            printf("%ld\n", eval(r.output));
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(readLine);
    }

    cleanup_parser();

    return 0;
}
