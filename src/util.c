#include "util.h"

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
        case TYPE_INT: printf("%ld", r->result.integer); break;
        case TYPE_DEC: printf("%Lf", r->result.decimal); break;
        case TYPE_SYM: printf("%s", r->result.symbol); break;
        case TYPE_SEXPR:
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
        case TYPE_ERR: printf("ERROR: %s", r->result.error); break;
        default:
            printf("Trying to print Result of unknown type: %d\n", r->type);
            break;
    }
}
void printResult(Result* r) {
    _printResult(r);
    printf("\n");
}
