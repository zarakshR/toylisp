#include "util.h"

static void _printAST(const mpc_ast_t* const node, const int level) {
    for (int i = 0; i < level; i++) { printf("\t"); }
    printf("%s:%s\t-->(%d)\n", strcmp(node->tag, "") ? node->tag : "/",
           strcmp(node->contents, "") ? node->contents : "_",
           node->children_num);
    for (int i = 0; i < node->children_num; i++) {
        _printAST(node->children[i], level + 1);
    }
}
void printAST(const mpc_ast_t* const node) {
    _printAST(node, 0);
}

static void _printResult(const Result* const r) {
    switch (r->type) {
        case TYPE_INT: printf("%ld", r->result.integer); break;
        case TYPE_DEC: printf("%Lf", r->result.decimal); break;
        case TYPE_SYM: printf("%s", r->result.symbol); break;
        case TYPE_SEXPR:
            printf("(");
            if (r->result.list.count > 0) {
                for (size i = 0; i < r->result.list.count - 1; i++) {
                    _printResult(r->result.list.cell[i]);
                    printf(" ");
                }
                _printResult(r->result.list.cell[r->result.list.count - 1]);
            }
            printf(")");
            break;
        case TYPE_QUOTE:
            printf("{");
            if (r->result.list.count > 0) {
                for (size i = 0; i < r->result.list.count - 1; i++) {
                    _printResult(r->result.list.cell[i]);
                    printf(" ");
                }
                _printResult(r->result.list.cell[r->result.list.count - 1]);
            }
            printf("}");
            break;
        case TYPE_ERR: printf("<%s>", r->result.error); break;
        default:
            printf("Trying to print Result of unknown type: %d\n", r->type);
            break;
    }
}
void printResult(const Result* const r) {
    _printResult(r);
    printf("\n");
}
