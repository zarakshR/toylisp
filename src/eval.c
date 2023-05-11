#include "eval.h"

Result* valResult(long x) {
    Result* res         = malloc(sizeof(Result));
    res->type           = VAL_INT;
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

// Will recursively free everythin in a sexpr
void resultFree(Result* res) {
    switch (res->type) {
        case VAL_INT: break;
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

Result* evalSym(Result* sexpr) {

    if (not(sexpr->result.list.count is 3)) {
        return errResult("Arity is not 2");
    }

    Result* sym  = sexpr->result.list.cell[0];
    Result* arg1 = sexpr->result.list.cell[1];
    Result* arg2 = sexpr->result.list.cell[2];

    if (not(arg1->type is VAL_INT and arg2->type is VAL_INT)) {
        return errResult("Symbol error");
    }

    long x = arg1->result.integer;
    long y = arg2->result.integer;
    long res;

    switch (parseSym(sym->result.symbol)) {
        case ADD:;
            if (__builtin_saddl_overflow(x, y, &res)) {
                return errResult(INT_FLOW);
            } else {
                return valResult(res);
            }
        case SUB:
            if (__builtin_ssubl_overflow(x, y, &res)) {
                return errResult(INT_FLOW);
            } else {
                return valResult(res);
            }
        case MUL:
            if (__builtin_smull_overflow(x, y, &res)) {
                return errResult(INT_FLOW);
            } else {
                return valResult(res);
            }
        case DIV:
            return y == 0 ? errResult(DIV_ZERO) : decResult(x / (double)y);
        case POW:;
            long acc = 1;
            for (int i = 0; i < y; i++) {
                if (__builtin_smull_overflow(acc, x, &acc)) {
                    return errResult(INT_FLOW);
                }
            }
            return valResult(acc);
        case MOD: return y == 0 ? errResult(DIV_ZERO) : valResult(x % y);
        case MIN: return valResult(x < y ? x : y);
        case MAX: return valResult(x > y ? x : y);
        default: return errResult("Symbol not found"); break;
    }
}

Result* eval(mpc_ast_t* node) {
    switch (parseTag(node->tag)) {
        case INT: {
            long val = strtol(node->contents, NULL, 0);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the
            // language grammar ensures valid numbers only.
            return valResult(val);
        }

        case DEC: {
            long double val = strtold(node->contents, NULL);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the
            // language grammar ensures valid numbers only.
            return decResult(val);
        }

        case SYM: return symResult(node->contents);

        case SEXPR:

            // Empty s-expression. The opening and closing brackets each counts
            // for one child.
            if (node->children_num == 2) { return sexprResult(); }

            // Single cell s-expression. The opening and closing brackets each
            // counts for one child.
            if (node->children_num == 3) { return eval(node->children[1]); }

            Result* sexpr = sexprResult();

            sexpr->result.list.cell = malloc(sizeof(Result*));

            // We loop from 1 to children_num - 1 since the first and last
            // elements of an s-expression are the '(' and ')' characters which
            // we can ignore
            for (int i = 1; i < node->children_num - 1; i++) {
                sexpr->result.list.count++;
                sexpr->result.list.cell =
                    realloc(sexpr->result.list.cell,
                            sizeof(Result*) * sexpr->result.list.count);
                sexpr->result.list.cell[i - 1] = eval(node->children[i]);
            }

            if (sexpr->result.list.cell[0]->type is VAL_SYM) {
                return evalSym(sexpr);
            }

            return sexpr;

        case ROOT: return eval(node->children[1]);

        default: assert(0 && "unreachable code reached in eval()");
    }
    assert(0 && "unreachable code reached in eval()");
}
