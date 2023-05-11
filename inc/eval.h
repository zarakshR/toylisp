#ifndef LITHP__EVAL_H
#define LITHP__EVAL_H

#include <utils.h>
#include <assert.h>

#include "mpc.h"
#include "parser.h"

typedef enum { VAL_INT, VAL_DEC, VAL_SYM, VAL_SEXPR, VAL_ERR } VAL_TYPE;

// Error strings
#define DIV_ZERO  "Division by zero"
#define ARG_COUNT "Wrong no. of arguments"
#define INT_FLOW  "Integer over/underflow"

typedef struct Result {
    VAL_TYPE type;
    union {
        long int integer;
        long double decimal;
        char* error;
        char* symbol;
        // An s-expression with size information
        struct {
            size count;
            struct Result** cell;
        } list;
    } result;
} Result;

void resultFree(Result*);
Result* evalSym(Result* sexpr);
Result* eval(mpc_ast_t*);

#endif
