#include "types.h"

Result* valResult(const long x) {
    Result* res         = malloc(sizeof(Result));
    res->type           = TYPE_INT;
    res->result.integer = x;
    return res;
}

Result* symResult(const char* const sym) {
    Result* res        = malloc(sizeof(Result));
    res->type          = TYPE_SYM;
    res->result.symbol = malloc(strlen(sym) + 1);
    strcpy(res->result.symbol, sym);
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
    res->result.error = malloc(strlen(err) + 1);
    strcpy(res->result.error, err);
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
        default: assert(0 && "Attempted free of unknown type");
    }
    free(res);
}

void resultListAppend(Result* const list, Result* const res) {
    list->result.list.cell =
        realloc(list->result.list.cell,
                sizeof(Result*) * (list->result.list.count + 1));
    list->result.list.cell[list->result.list.count] = res;
    list->result.list.count++;
}
