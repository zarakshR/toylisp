#ifndef LITHP__UTIL_H
#define LITHP__UTIL_H

#include <stdio.h>
#include <assert.h>

#include "mpc.h"
#include "types.h"

#define PANIC(msg) assert(0 && msg)

void printAST(const mpc_ast_t* const);
void printResult(const Result* const);

#endif
