#ifndef LITHP__EVAL_H
#define LITHP__EVAL_H

#include <utils.h>
#include <assert.h>

#include "mpc.h"
#include "parser.h"

typedef enum { INT_VALUE, DEC_VALUE, ERROR } RES_TYPE;
typedef enum { DIV_ZERO, ARG_COUNT, INT_FLOW } ERR_CODE;

typedef struct {
    RES_TYPE type;
    union {
        long int ivalue;
        long double dvalue;
        ERR_CODE evalue;
    } result;
} Result;

Result evalOp(char* op, long, long);
Result eval(mpc_ast_t*);

#endif
