#ifndef TOYLISP__PARSER_H
#define TOYLISP__PARSER_H

#include "mpc.h"
#include "types.h"

#include "util.h"

extern mpc_parser_t* Integer;
extern mpc_parser_t* Number;
extern mpc_parser_t* Symbol;
extern mpc_parser_t* Expr;
extern mpc_parser_t* Sexpr;
extern mpc_parser_t* Quote;
extern mpc_parser_t* Program;

void cleanup_parser(void);
void setup_parser(void);

TAG parseTag(const char*);
Result* parseAST(const mpc_ast_t*);
void printAST(const mpc_ast_t* const node);

#endif
