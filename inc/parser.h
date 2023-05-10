#ifndef LITHP__PARSER_H
#define LITHP__PARSER_H

#include <utils.h>
#include <assert.h>

#include "mpc.h"

extern mpc_parser_t* Integer;
extern mpc_parser_t* Decimal;
extern mpc_parser_t* Number;
extern mpc_parser_t* Operator;
extern mpc_parser_t* Expr;
extern mpc_parser_t* Program;

typedef enum { NUM, DEC, EXPR, ROOT } SYMBOL;
typedef enum { ADD, SUB, MUL, DIV, POW, MOD, MIN, MAX } OP;

void cleanup_parser(void);
void setup_parser(void);

OP parseOP(char*);
SYMBOL parseTag(char*);

#endif
