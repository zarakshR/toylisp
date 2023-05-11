#ifndef LITHP__EVAL_H
#define LITHP__EVAL_H

#include <utils.h>
#include <assert.h>

#include "mpc.h"
#include "parser.h"

// Error strings
#define DIV_ZERO  "Division by zero"
#define ARG_COUNT "Wrong no. of arguments"
#define INT_FLOW  "Integer over/underflow"

void resultFree(Result*);
Result* evalSym(Result* sexpr);
Result* eval(Result*);
Result* parseAST(mpc_ast_t*);

#endif
