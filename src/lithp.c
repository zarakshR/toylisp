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

void _printResult(Result* r) {
    switch (r->type) {
        case VAL_INT: printf("%ld", r->result.integer); break;
        case VAL_DEC: printf("%Lf", r->result.decimal); break;
        case VAL_SYM: printf("SYMBOL: %s", r->result.symbol); break;
        case VAL_SEXPR:
            printf("(");
            // Print only upto the *second-last* element so we can avoid adding
            // a space after the last one. This is just for prettiness.
            if (not(r->result.list.cell is NULL)) {
                for (size i = 0; i < r->result.list.count - 1; i++) {
                    _printResult(r->result.list.cell[i]);
                    printf(" ");
                }
                // Now print the last element with adding a space after it
                _printResult(r->result.list.cell[r->result.list.count - 1]);
            }
            printf(")");
            break;
        case VAL_ERR: printf("ERROR: %s", r->result.error); break;
        default: assert(0 && "unreachable code reached in _printResult()");
    }
}
void printResult(Result* r) {
    _printResult(r);
    printf("\n");
}

int main() {
    setup_parser();

    loop {
        mpc_result_t r;

        char* readLine = readline("> ");
        if (readLine is NULL) { break; }
        add_history(readLine);

        if (mpc_parse("<stdin>", readLine, Program, &r)) {
            printAST(r.output);
            Result* eval_result = eval(r.output);
            printResult(eval_result);
            resultFree(eval_result);
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
