#include "eval.h"

Result valResult(long x) {
    Result res = {.type = INT_VALUE, .result.ivalue = x};
    return res;
}

Result dubResult(long double x) {
    Result res = {.type = DEC_VALUE, .result.dvalue = x};
    return res;
}

Result errResult(ERR_CODE err) {
    Result res = {.type = ERROR, .result.evalue = err};
    return res;
}

Result evalOp(char* op, long x, long y) {
    switch (parseOP(op)) {
        long res;
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
        case DIV: return y == 0 ? errResult(DIV_ZERO) : valResult(x / y);
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
        default: assert(0 && "unreachable code reached in evalOp()");
    }
}

Result eval(mpc_ast_t* node) {
    switch (parseTag(node->tag)) {
        case NUM: {
            long val = strtol(node->contents, NULL, 0);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the
            // language grammar ensures valid numbers only.
            return valResult(val);
        }

        case DEC: {
            long double val = strtold(node->contents, NULL);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the language
            // grammar ensures valid numbers only.
            return dubResult(val);
        }

        case EXPR:;

            // Only accept 2-arity functions: '(' + op + arg1 + arg2 + ')' is 5
            if (node->children_num != 5) { return errResult(ARG_COUNT); }

            // Grammar guarantees first arg is the operation
            char* op = node->children[1]->contents;

            Result res1 = eval(node->children[2]);
            if (res1.type is ERROR) { return res1; }

            Result res2 = eval(node->children[3]);
            if (res2.type is ERROR) { return res2; }

            // Only evaluate op if both arguments are ints
            if (res1.type is INT_VALUE and res2.type is INT_VALUE) {
                return evalOp(op, (long)res1.result.ivalue,
                              (long)res2.result.ivalue);
            }

            assert(0 && "Uninmplemented handling of decimal arguments");

        case ROOT: return eval(node->children[1]);
        default: assert(0 && "unreachable code reached in eval()");
    }
    assert(0 && "unreachable code reached in eval()");
}
