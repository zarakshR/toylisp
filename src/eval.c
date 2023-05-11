#include "eval.h"
#include "types.h"

Result* evalSym(Result* sexpr) {

    if (not(sexpr->result.list.count is 3)) {
        return errResult("Arity is not 2");
    }

    Result* sym  = sexpr->result.list.cell[0];
    Result* arg1 = sexpr->result.list.cell[1];
    Result* arg2 = sexpr->result.list.cell[2];

    if (not(arg1->type is TYPE_INT and arg2->type is TYPE_INT)) {
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

Result* parseAST(mpc_ast_t* node) {
    return NULL;
}
