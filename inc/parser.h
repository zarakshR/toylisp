#ifndef LITHP__PARSER_H
#define LITHP__PARSER_H

#include "mpc.h"

extern mpc_parser_t* Number;
extern mpc_parser_t* Operator;
extern mpc_parser_t* Expr;
extern mpc_parser_t* Lispy;

void cleanup_parser(void);
void setup_parser(void);

#endif
