#include "parser.h"

mpc_parser_t* Integer;
mpc_parser_t* Decimal;
mpc_parser_t* Number;
mpc_parser_t* Symbol;
mpc_parser_t* Expr;
mpc_parser_t* Sexpr;
mpc_parser_t* Quote;
mpc_parser_t* Program;

void setup_parser(void) {
    // Initialize Parsers
    Integer = mpc_new("integer");
    Decimal = mpc_new("decimal");
    Number  = mpc_new("number");
    Symbol  = mpc_new("symbol");
    Expr    = mpc_new("expr");
    Sexpr   = mpc_new("sexpr");
    Quote   = mpc_new("quote");
    Program = mpc_new("program");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT, " \
        integer : /-?[0-9]+/ ; \
        decimal : /-?[0-9]*\\.[0-9]+/ ; \
        number  : <decimal> | <integer> ; \
        symbol  : '+' | '-' | '*' | '/' | \"_/\" \
                 | '%' | '^' | \"min\" | \"max\"; \
        expr    : <number> | <symbol> | <sexpr> | <quote> ; \
        quote   : '{' <expr>* '}' ; \
        sexpr   : '(' <expr>* ')' ; \
        program : /^/ <expr>* /$/ ; \
      ",
              Integer, Decimal, Number, Symbol, Expr, Sexpr, Quote, Program);
}

void cleanup_parser(void) {
    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Integer, Decimal, Number, Symbol, Expr, Sexpr, Quote,
                Program);
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
    if (strstr(tag, "integer")) { return TAG_INT; }
    if (strstr(tag, "decimal")) { return TAG_DEC; }
    if (strstr(tag, "symbol")) { return TAG_SYM; }
    if (strstr(tag, "sexpr")) { return TAG_SEXPR; }
    if (strstr(tag, "quote")) { return TAG_QUOTE; }
    assert(0 && "unreachable code reached in parseTag()");
}
