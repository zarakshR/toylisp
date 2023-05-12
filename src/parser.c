#include "parser.h"

mpc_parser_t* Integer;
mpc_parser_t* Number;
mpc_parser_t* Symbol;
mpc_parser_t* Expr;
mpc_parser_t* Sexpr;
mpc_parser_t* Quote;
mpc_parser_t* Program;

void setup_parser(void) {
    // Initialize Parsers
    Integer = mpc_new("integer");
    Number  = mpc_new("number");
    Symbol  = mpc_new("symbol");
    Expr    = mpc_new("expr");
    Sexpr   = mpc_new("sexpr");
    Quote   = mpc_new("quote");
    Program = mpc_new("program");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT, " \
        integer : /-?[0-9]+/ ; \
        number  : <integer> ; \
        symbol  : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\" ; \
        expr    : <number> | <symbol> | <sexpr> | <quote> ; \
        quote   : '{' <expr>* '}' ; \
        sexpr   : '(' <expr>* ')' ; \
        program : /^/ <expr>* /$/ ; \
      ",
              Integer, Number, Symbol, Expr, Sexpr, Quote, Program);
}

void cleanup_parser(void) {
    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Integer, Number, Symbol, Expr, Sexpr, Quote, Program);
}

OP parseSym(const char* op) {
    if (not strcmp(op, "+")) { return ADD; }
    if (not strcmp(op, "-")) { return SUB; }
    if (not strcmp(op, "*")) { return MUL; }
    if (not strcmp(op, "/")) { return DIV; }
    if (not strcmp(op, "^")) { return POW; }
    if (not strcmp(op, "%")) { return MOD; }
    if (not strcmp(op, "min")) { return MIN; }
    if (not strcmp(op, "max")) { return MAX; }
    PANIC("unreachable code reached in parseOp()");
}

SYMBOL parseTag(const char* tag) {
    if (strstr(tag, "integer")) { return TAG_INT; }
    if (strstr(tag, "symbol")) { return TAG_SYM; }
    if (strstr(tag, "sexpr")) { return TAG_SEXPR; }
    if (strstr(tag, "quote")) { return TAG_QUOTE; }
    PANIC("unreachable code reached in parseTag()");
}
