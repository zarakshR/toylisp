#include "parser.h"

mpc_parser_t* Number;
mpc_parser_t* Operator;
mpc_parser_t* Expr;
mpc_parser_t* Program;

void setup_parser(void) {
    // Initialize Parsers
    Number   = mpc_new("number");
    Operator = mpc_new("operator");
    Expr     = mpc_new("expr");
    Program  = mpc_new("program");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT, " \
        number   : /-?[0-9]+/ ; \
        operator : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\" ; \
        expr     : <number> | '(' <operator> <expr>+ ')' ; \
        program    : /^/ <expr>+ /$/ ; \
      ",
              Number, Operator, Expr, Program);
}

void cleanup_parser(void) {
    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, Program);
}
