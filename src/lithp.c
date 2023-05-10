#include "lithp.h"

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
        case INT_FLOW: return "Integer over/underflow";
        default: return "Unknown error";
    }
}

void printResult(Result r) {
    switch (r.type) {
        case INT_VALUE: printf("%ld\n", r.result.ivalue); break;
        case DEC_VALUE: printf("%Lf\n", r.result.dvalue); break;
        case ERROR: printf("%s\n", reprError(r.result.evalue)); break;
        default: assert(0 && "unreachable code reached in printResult()");
    }
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
