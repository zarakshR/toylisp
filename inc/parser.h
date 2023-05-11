#ifndef LITHP__PARSER_H
#define LITHP__PARSER_H

#include <utils.h>
#include <assert.h>

#include "mpc.h"
#include "types.h"

extern mpc_parser_t* Integer;
extern mpc_parser_t* Decimal;
extern mpc_parser_t* Number;
extern mpc_parser_t* Symbol;
extern mpc_parser_t* Expr;
extern mpc_parser_t* Sexpr;
extern mpc_parser_t* Program;

typedef enum { ADD, SUB, MUL, DIV, POW, MOD, MIN, MAX } OP;

void cleanup_parser(void);
void setup_parser(void);

OP parseSym(char*);
SYMBOL parseTag(char*);

#endif
