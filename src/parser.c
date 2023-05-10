#include "parser.h"

mpc_parser_t* Integer;
mpc_parser_t* Decimal;
mpc_parser_t* Number;
mpc_parser_t* Operator;
mpc_parser_t* Expr;
mpc_parser_t* Program;

void setup_parser(void) {
    // Initialize Parsers
    Integer  = mpc_new("integer");
    Decimal  = mpc_new("decimal");
    Number   = mpc_new("number");
    Operator = mpc_new("operator");
    Expr     = mpc_new("expr");
    Program  = mpc_new("program");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT, " \
        integer   : /-?[0-9]+/ ; \
        decimal  : /-?[0-9]*\\.[0-9]+/ ; \
        number   : <decimal> | <integer> ; \
        operator : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\" ; \
        expr     : <number> | '(' <operator> <expr>+ ')' ; \
        program    : /^/ <expr>+ /$/ ; \
      ",
              Integer, Decimal, Number, Operator, Expr, Program);
}

void cleanup_parser(void) {
    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Integer, Decimal, Number, Operator, Expr, Program);
}

OP parseOP(char* op) {
    if (not strcmp(op, "+")) { return ADD; }
    if (not strcmp(op, "-")) { return SUB; }
    if (not strcmp(op, "*")) { return MUL; }
    if (not strcmp(op, "/")) { return DIV; }
    if (not strcmp(op, "^")) { return POW; }
    if (not strcmp(op, "%")) { return MOD; }
    if (not strcmp(op, "min")) { return MIN; }
    if (not strcmp(op, "max")) { return MAX; }
    assert(0 && "unreachable code reached in parseOp()");
}

SYMBOL parseTag(char* tag) {
    if (not strcmp(tag, "expr|number|integer|regex")) { return NUM; }
    if (not strcmp(tag, "expr|number|decimal|regex")) { return DEC; }
    if (not strcmp(tag, "expr|>")) { return EXPR; }
    if (not strcmp(tag, ">")) { return ROOT; }
    assert(0 && "unreachable code reached in parseTag()");
}
