#include "types.h"

// Constructors for the different types
Result* valResult(const long x) {
    Result* res         = malloc(sizeof(Result));
    res->type           = TYPE_INT;
    res->result.integer = x;
    return res;
}

Result* symResult(const char* const sym) {
    Result* res        = malloc(sizeof(Result));
    res->type          = TYPE_SYM;
    res->result.symbol = strdup(sym);
    return res;
}

Result* sexprResult() {
    Result* res            = malloc(sizeof(Result));
    res->type              = TYPE_SEXPR;
    res->result.list.count = 0;
    res->result.list.cell  = NULL;
    return res;
}

Result* quoteResult() {
    Result* res            = malloc(sizeof(Result));
    res->type              = TYPE_QUOTE;
    res->result.list.count = 0;
    res->result.list.cell  = NULL;
    return res;
}

Result* errResult(const char* const err) {
    Result* res       = malloc(sizeof(Result));
    res->type         = TYPE_ERR;
    res->result.error = strdup(err);
    return res;
}

// Will recursively free everything in a sexpr
void resultFree(Result* res) {
    switch (res->type) {
        case TYPE_INT: break;
        case TYPE_SYM: free(res->result.symbol); break;
        case TYPE_SEXPR:
        case TYPE_QUOTE:
            for (size i = 0; i < res->result.list.count; i++) {
                resultFree(res->result.list.cell[i]);
            }
            free(res->result.list.cell);
            res->result.list.count = 0;
            break;
        case TYPE_ERR: free(res->result.error); break;
        default: PANIC("Attempted free of unknown type");
    }
    free(res);
}

// Appends a single Result to a list.
void resultListAppend(Result* const list, Result* const res) {
    list->result.list.cell =
        realloc(list->result.list.cell,
                sizeof(Result*) * (list->result.list.count + 1));
    list->result.list.cell[list->result.list.count] = res;
    list->result.list.count++;
}

static void _printResult(const Result* const r) {
    switch (r->type) {
        case TYPE_INT: printf("%ld", r->result.integer); break;
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

// Lookup table for builtin functions
OP parseSym(const char* op) {
    if (not strcmp(op, "+")) { return ADD; }
    if (not strcmp(op, "-")) { return SUB; }
    if (not strcmp(op, "*")) { return MUL; }
    if (not strcmp(op, "/")) { return DIV; }
    if (not strcmp(op, "^")) { return POW; }
    if (not strcmp(op, "%")) { return MOD; }
    if (not strcmp(op, "min")) { return MIN; }
    if (not strcmp(op, "max")) { return MAX; }
    PANIC("unreachable code reached in parseOp()");
}
