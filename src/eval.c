#include "eval.h"

// evalSym must free the sexpr it is passed and return an alternate Result*
Result* evalSym(Result* sexpr) {

    if (not(sexpr->result.list.count is 3)) {
        resultFree(sexpr);
        return errResult("ARITY IS NOT 2");
    }

    Result* sym  = sexpr->result.list.cell[0];
    Result* arg1 = sexpr->result.list.cell[1];
    Result* arg2 = sexpr->result.list.cell[2];

    // Currently only accepts if both args are ints
    if (not(arg1->type is TYPE_INT and arg2->type is TYPE_INT)) {
        resultFree(sexpr);
        return errResult("NON-INT ARGUMENTS NOT SUPPORTED");
    }

    long x = arg1->result.integer;
    long y = arg2->result.integer;
    long res;

    // Return error if first arg is not a symbol
    if (not(sym->type is TYPE_SYM)) {
        resultFree(sexpr);
        return errResult("ATTEMPTING EVALUATION OF NON-SYMBOL");
    }

    switch (parseSym(sym->result.symbol)) {
        case ADD:;
            if (__builtin_saddl_overflow(x, y, &res)) {
                resultFree(sexpr);
                return errResult(INT_FLOW);
            } else {
                resultFree(sexpr);
                return valResult(res);
            }
        case SUB:
            if (__builtin_ssubl_overflow(x, y, &res)) {
                resultFree(sexpr);
                return errResult(INT_FLOW);
            } else {
                resultFree(sexpr);
                return valResult(res);
            }
        case MUL:
            if (__builtin_smull_overflow(x, y, &res)) {
                resultFree(sexpr);
                return errResult(INT_FLOW);
            } else {
                resultFree(sexpr);
                return valResult(res);
            }
        case DIV:
            if (y == 0) {
                resultFree(sexpr);
                return errResult(DIV_ZERO);
            }
            resultFree(sexpr);
            return decResult(x / y);
        case POW:;
            long acc = 1;
            for (int i = 0; i < y; i++) {
                if (__builtin_smull_overflow(acc, x, &acc)) {
                    resultFree(sexpr);
                    return errResult(INT_FLOW);
                }
            }
            resultFree(sexpr);
            return valResult(acc);
        case MOD:
            if (y == 0) {
                resultFree(sexpr);
                return errResult(DIV_ZERO);
            }
            resultFree(sexpr);
            return valResult(x % y);
        case MIN: resultFree(sexpr); return valResult(x < y ? x : y);
        case MAX: resultFree(sexpr); return valResult(x > y ? x : y);
        default: resultFree(sexpr); return errResult("SYMBOL NOT FOUND");
    }
}

Result* eval(Result* expr) {
    switch (expr->type) {
        case TYPE_INT: return expr;
        case TYPE_DEC: return expr;
        case TYPE_SYM: return expr;
        case TYPE_ERR: return expr;
        case TYPE_SEXPR:;

            // Evaluate all the children.
            // TODO: Find a way to shortcircuit on errors
            for (size i = 0; i < expr->result.list.count; i++) {
                expr->result.list.cell[i] = eval(expr->result.list.cell[i]);
            }

            // Evaluate ourselves;
            return evalSym(expr);

        case TYPE_QUOTE: return expr;
        default: assert(0 && "unreachable code reached in eval()");
    }
}

Result* parseAST(mpc_ast_t* node) {
    switch (parseTag(node->tag)) {
        case TAG_INT: {
            long val = strtol(node->contents, NULL, 0);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the
            // language grammar ensures valid numbers only.
            return valResult(val);
        }

        case TAG_DEC: {
            long double val = strtold(node->contents, NULL);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the
            // language grammar ensures valid numbers only.
            return decResult(val);
        }

        case TAG_SYM: return symResult(node->contents);

        case TAG_SEXPR:;

            Result* sexpr = sexprResult();

            // We want to loop from 1 to children_num-1 since the first and last
            // children are going to be '(' and ')' respectively
            for (int i = 1; i < node->children_num - 1; i++) {
                resultListAppend(sexpr, parseAST(node->children[i]));
            }

            return sexpr;

        case TAG_QUOTE:;

            // A quote has the same internal representation as a sexpr
            Result* quote = sexprResult();

            // We want to loop from 1 to children_num-1 since the first and last
            // children are going to be '`' and '`' respectively
            for (int i = 1; i < node->children_num - 1; i++) {
                resultListAppend(quote, parseAST(node->children[i]));
            }

            return quote;

        default: assert(0 && "unreachable code reached in parseAST()");
    }
    assert(0 && "unreachable code reached in parseAST()");
}
