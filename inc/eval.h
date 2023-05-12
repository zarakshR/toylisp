#ifndef LITHP__EVAL_H
#define LITHP__EVAL_H

#include <utils.h>
#include <assert.h>

#include "mpc.h"
#include "parser.h"
#include "types.h"

// Error strings
#define DIV_ZERO  "DIVISION BY ZERO"
#define ARG_COUNT "WRONG NO. OF ARGUMENTS"
#define INT_FLOW  "INTEGER OVER/UNDERFLOW"

// Free expression and return error
#define ERR_OUT(tofree, err_message)                                           \
    {                                                                          \
        resultFree(tofree);                                                    \
        return errResult(err_message);                                         \
    }

// Free expression and return value
#define VAL_OUT(tofree, value)                                                 \
    {                                                                          \
        resultFree(tofree);                                                    \
        return valResult(value);                                               \
    }

// Return arity mismatch error if arity is not what we expected
#define ASSERT_ARITY(expr, arity)                                              \
    {                                                                          \
        if (expr->result.list.count isnot arity + 1) {                         \
            ERR_OUT(expr, "ARITY MISMATCH");                                   \
        }                                                                      \
    }

Result* eval(Result*);
Result* parseAST(const mpc_ast_t*);

#endif
