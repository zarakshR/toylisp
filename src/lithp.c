#ifndef __GNUC__
#error "GCC required"
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <utils.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "lithp.h"
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

const char* reprError(ERR_CODE err) {
    switch (err) {
        case DIV_ZERO: return "Division by zero";
        case ARG_COUNT: return "Wrong number of arguments";
        case INT_OVERFLOW: return "Integer overflow";
        default: return "Unknown error";
    }
}

void printResult(Result r) {
    switch (r.type) {
        case VALUE: printf("%ld\n", r.result.value); break;
        case ERROR: printf("%s\n", reprError(r.result.error)); break;
        default: assert(0 && "unreachable code reached in printResult()");
    }
}

Result valResult(long x) {
    return (Result){VALUE, {x}};
}

Result errResult(ERR_CODE err) {
    return (Result){ERROR, {err}};
}

OP parseOP(char* op) {
    if (not strcmp(op, "+")) { return ADD; }
    if (not strcmp(op, "-")) { return SUB; }
    if (not strcmp(op, "*")) { return MUL; }
    if (not strcmp(op, "/")) { return DIV; }
    if (not strcmp(op, "^")) { return POW; }
    if (not strcmp(op, "%")) { return MOD; }
    if (not strcmp(op, "min")) { return MIN; }
    if (not strcmp(op, "max")) { return MAX; }
    assert(0 && "unreachable code reached in parseOp()");
}

Result evalOp(char* op, long x, long y) {
    switch (parseOP(op)) {
        long res;
        case ADD:;
            if (__builtin_saddl_overflow(x, y, &res)) {
                return errResult(INT_OVERFLOW);
            } else {
                return valResult(res);
            }
        case SUB:
            if (__builtin_ssubl_overflow(x, y, &res)) {
                return errResult(INT_OVERFLOW);
            } else {
                return valResult(res);
            }
        case MUL:
            if (__builtin_smull_overflow(x, y, &res)) {
                return errResult(INT_OVERFLOW);
            } else {
                return valResult(res);
            }
        case DIV: return y == 0 ? errResult(DIV_ZERO) : valResult(x / y);
        case POW:;
            long acc = 1;
            for (int i = 0; i < y; i++) {
                if (__builtin_smull_overflow(acc, x, &acc)) {
                    return errResult(INT_OVERFLOW);
                }
            }
            return valResult(acc);
        case MOD:;
            return y == 0 ? errResult(DIV_ZERO) : valResult(x - y * (x / y));
        case MIN: return valResult(x < y ? x : y);
        case MAX: return valResult(x > y ? x : y);
        default: assert(0 && "unreachable code reached in evalOp()");
    }
}

SYMBOL parseTag(char* tag) {
    if (not strcmp(tag, "expr|number|regex")) { return NUM; }
    if (not strcmp(tag, "expr|>")) { return EXPR; }
    if (not strcmp(tag, ">")) { return ROOT; }
    assert(0 && "unreachable code reached in parseTag()");
}

Result eval(mpc_ast_t* node) {
    switch (parseTag(node->tag)) {
        case NUM:;
            char* endptr;
            long val = strtol(node->contents, &endptr, 0);
            if (errno is ERANGE) { return errResult(INT_OVERFLOW); }
            // We shouldn't need to check for other failures since the language
            // grammar ensures valid numbers only.
            return valResult(val);

        case EXPR:;

            if (strcmp(node->children[2]->contents, ")") is 0
                or strcmp(node->children[3]->contents, ")") is 0) {
                return errResult(ARG_COUNT);
            }

            char* op = node->children[1]->contents;

            Result res1 = eval(node->children[2]);
            if (res1.type is ERROR) { return res1; }

            Result res2 = eval(node->children[3]);
            if (res2.type is ERROR) { return res2; }

            return evalOp(op, (long)res1.result.value, (long)res2.result.value);

        case ROOT: return eval(node->children[1]);
        default: assert(0 && "unreachable code reached in eval()");
    }
    assert(0 && "unreachable code reached in eval()");
}

int main() {
    setup_parser();

    loop {
        mpc_result_t r;

        char* readLine = readline("> ");
        if (readLine is NULL) { break; }
        add_history(readLine);

        if (mpc_parse("<stdin>", readLine, Program, &r)) {
            Result eval_result = eval(r.output);
            printResult(eval_result);
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
