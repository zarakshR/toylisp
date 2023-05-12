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

Result* eval(Result*);
Result* parseAST(const mpc_ast_t*);

#endif
