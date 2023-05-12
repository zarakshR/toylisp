#ifndef TOYLISP__EVAL_H
#define TOYLISP__EVAL_H

#include "types.h"

#include "util.h"

// Free expression and return error
#define ERR_OUT(EXPRESSION_TO_FREE, ERROR_MESSAGE)                             \
    {                                                                          \
        resultFree(EXPRESSION_TO_FREE);                                        \
        return errResult(ERROR_MESSAGE);                                       \
    }

// Free expression and return value
#define VAL_OUT(EXPRESSION_TO_FREE, VALUE_TO_INSTANTIATE)                      \
    {                                                                          \
        resultFree(EXPRESSION_TO_FREE);                                        \
        return valResult(VALUE_TO_INSTANTIATE);                                \
    }

// Return arity mismatch error if arity is not what we expected
#define ASSERT_ARITY(EXPRESSION, ARITY_EXPECTED)                               \
    {                                                                          \
        if (SIZE_OF(EXPRESSION) isnot ARITY_EXPECTED + 1) {                    \
            ERR_OUT(EXPRESSION, "ARITY MISMATCH");                             \
        }                                                                      \
    }

// Checks if the n'th argument is of type T. Expects a TYPE enum
#define ASSERT_TYPE(EXPRESSION, INDEX, TYPE_EXPECTED)                          \
    {                                                                          \
        if (ELEM_AT(EXPRESSION, INDEX)->type isnot TYPE_EXPECTED) {            \
            ERR_OUT(EXPRESSION, "TYPE ERROR")                                  \
        }                                                                      \
    }

Result* eval(Result*);

#endif
