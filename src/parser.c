#include "parser.h"

mpc_parser_t* Integer;
mpc_parser_t* Decimal;
mpc_parser_t* Number;
mpc_parser_t* Symbol;
mpc_parser_t* Expr;
mpc_parser_t* Sexpr;
mpc_parser_t* Program;

void setup_parser(void) {
    // Initialize Parsers
    Integer = mpc_new("integer");
    Decimal = mpc_new("decimal");
    Number  = mpc_new("number");
    Symbol  = mpc_new("symbol");
    Expr    = mpc_new("expr");
    Sexpr   = mpc_new("sexpr");
    Program = mpc_new("program");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT, " \
        integer : /-?[0-9]+/ ; \
        decimal : /-?[0-9]*\\.[0-9]+/ ; \
        number  : <decimal> | <integer> ; \
        symbol  : '+' | '-' | '*' | '/' | \"_/\" \
                 | '%' | '^' | \"min\" | \"max\"; \
        expr    : <number> | <symbol> | <sexpr> ; \
        sexpr   : '(' <expr>* ')' ; \
        program : /^/ <sexpr>* /$/ ; \
      ",
              Integer, Decimal, Number, Symbol, Expr, Sexpr, Program);
}

void cleanup_parser(void) {
    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Integer, Decimal, Number, Symbol, Expr, Sexpr, Program);
}

OP parseSym(char* op) {
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
    if (not strcmp(tag, "expr|number|integer|regex")) { return INT; }
    if (not strcmp(tag, "expr|number|decimal|regex")) { return DEC; }
    if (not strcmp(tag, "expr|>")) { return EXPR; }
    if (not strcmp(tag, "expr|symbol|char")
        or not strcmp(tag, "expr|symbol|string")) {
        return SYM;
    }
    if (not strcmp(tag, "expr|sexpr|>") or not strcmp(tag, "sexpr|>")) {
        return SEXPR;
    }
    if (not strcmp(tag, ">")) { return ROOT; }
    assert(0 && "unreachable code reached in parseTag()");
}
