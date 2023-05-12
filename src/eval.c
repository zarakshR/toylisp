#include "eval.h"

// evalSym must free the sexpr it is passed and return an alternate Result*
// This function is macro-heavy, refer eval.h and type.h for all relevant
// macro definitions
static Result* evalSym(Result* const sexpr) {

    // Return error if first arg is not a symbol
    ASSERT_TYPE(sexpr, 0, TYPE_SYM);

    Result* sym = sexpr->result.list.cell[0];

    switch (parseSym(sym->result.symbol)) {
        case ADD: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x   = sexpr->result.list.cell[1]->result.integer;
            long y   = sexpr->result.list.cell[2]->result.integer;
            long res = 1;

            if (__builtin_saddl_overflow(x, y, &res)) {
                ERR_OUT(sexpr, INT_FLOW);
            } else {
                VAL_OUT(sexpr, res);
            }
        }

        case SUB: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x   = sexpr->result.list.cell[1]->result.integer;
            long y   = sexpr->result.list.cell[2]->result.integer;
            long res = 1;

            if (__builtin_ssubl_overflow(x, y, &res)) {
                ERR_OUT(sexpr, INT_FLOW);
            } else {
                VAL_OUT(sexpr, res);
            }
        }

        case MUL: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x   = sexpr->result.list.cell[1]->result.integer;
            long y   = sexpr->result.list.cell[2]->result.integer;
            long res = 1;

            if (__builtin_smull_overflow(x, y, &res)) {
                ERR_OUT(sexpr, INT_FLOW);
            } else {
                VAL_OUT(sexpr, res);
            }
        }

        case DIV: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x = sexpr->result.list.cell[1]->result.integer;
            long y = sexpr->result.list.cell[2]->result.integer;

            if (y == 0) { ERR_OUT(sexpr, DIV_ZERO); }
            VAL_OUT(sexpr, (x / y));
        }

        case POW: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x   = sexpr->result.list.cell[1]->result.integer;
            long y   = sexpr->result.list.cell[2]->result.integer;
            long res = 1;

            // TODO: Does not handle negative exponents.
            for (int i = 0; i < y; i++) {
                if (__builtin_smull_overflow(res, x, &res)) {
                    ERR_OUT(sexpr, INT_FLOW);
                }
            }
            VAL_OUT(sexpr, res);
        }

        case MOD: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x = sexpr->result.list.cell[1]->result.integer;
            long y = sexpr->result.list.cell[2]->result.integer;

            if (y == 0) { ERR_OUT(sexpr, DIV_ZERO); }
            VAL_OUT(sexpr, (x % y));
        }

        case MIN: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x = sexpr->result.list.cell[1]->result.integer;
            long y = sexpr->result.list.cell[2]->result.integer;

            VAL_OUT(sexpr, (x < y ? x : y));
        }

        case MAX: {
            ASSERT_ARITY(sexpr, 2);
            ASSERT_TYPE(sexpr, 1, TYPE_INT);
            ASSERT_TYPE(sexpr, 2, TYPE_INT);

            long x = sexpr->result.list.cell[1]->result.integer;
            long y = sexpr->result.list.cell[2]->result.integer;

            VAL_OUT(sexpr, (x > y ? x : y));
        }

        default: ERR_OUT(sexpr, "SYMBOL NOT FOUND");
    }
}

Result* eval(Result* expr) {
    switch (expr->type) {
        case TYPE_INT: return expr;
        case TYPE_SYM: return expr;
        case TYPE_ERR: return expr;
        case TYPE_SEXPR:;

            // Evaluate all the children.
            for (size i = 0; i < expr->result.list.count; i++) {
                expr->result.list.cell[i] = eval(expr->result.list.cell[i]);
                if (expr->result.list.cell[i]->type is TYPE_ERR) {
                    return expr;
                }
            }

            // Evaluate ourselves;
            return evalSym(expr);

        case TYPE_QUOTE: return expr;
        default: PANIC("Attempted to evaluate value of unknown type");
    }
}
