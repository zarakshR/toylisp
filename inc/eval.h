#ifndef TOYLISP__EVAL_H
#define TOYLISP__EVAL_H

#include "types.h"

#include "util.h"

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

// Checks if the n'th argument is of type T. Expects a TYPE enum
#define ASSERT_TYPE(expr, n, T)                                                \
    {                                                                          \
        if (expr->result.list.cell[n]->type isnot T) {                         \
            ERR_OUT(expr, "TYPE ERROR")                                        \
        }                                                                      \
    }

Result* eval(Result*);

#endif
