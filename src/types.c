#include "types.h"
#include "eval.h"

Result* valResult(long x) {
    Result* res         = malloc(sizeof(Result));
    res->type           = TYPE_INT;
    res->result.integer = x;
    return res;
}

Result* decResult(long double x) {
    Result* res         = malloc(sizeof(Result));
    res->type           = VAL_DEC;
    res->result.decimal = x;
    return res;
}

Result* symResult(char* sym) {
    Result* res        = malloc(sizeof(Result));
    res->type          = VAL_SYM;
    res->result.symbol = malloc(strlen(sym) + 1);
    strcpy(res->result.symbol, sym);
    return res;
}

Result* sexprResult() {
    Result* res            = malloc(sizeof(Result));
    res->type              = VAL_SEXPR;
    res->result.list.count = 0;
    res->result.list.cell  = NULL;
    return res;
}

Result* errResult(char* err) {
    Result* res       = malloc(sizeof(Result));
    res->type         = VAL_ERR;
    res->result.error = malloc(strlen(err) + 1);
    strcpy(res->result.error, err);
    return res;
}

// Will recursively free everything in a sexpr
void resultFree(Result* res) {
    switch (res->type) {
        case TYPE_INT: break;
        case VAL_DEC: break;
        case VAL_SYM: free(res->result.symbol); break;
        case VAL_SEXPR:
            for (size i = 0; i < res->result.list.count; i++) {
                resultFree(res->result.list.cell[i]);
            }
            free(res->result.list.cell);
            break;
        case VAL_ERR: free(res->result.error); break;
        default: break;
    }
    free(res);
}

void resultListAppend(Result* list, Result* res) {
    list->result.list.cell =
        realloc(list->result.list.cell,
                sizeof(Result*) * (list->result.list.count + 1));
    list->result.list.cell[list->result.list.count] = res;
    list->result.list.count++;
}
