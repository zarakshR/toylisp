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
        symbol  : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\" | \
                 \"list\" | \"car\" | \"cdr\" | \"join\" | \"eval\" ; \
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

TAG parseTag(const char* tag) {
    if (strstr(tag, "integer")) { return TAG_INT; }
    if (strstr(tag, "symbol")) { return TAG_SYM; }
    if (strstr(tag, "sexpr")) { return TAG_SEXPR; }
    if (strstr(tag, "quote")) { return TAG_QUOTE; }
    PANIC("unreachable code reached in parseTag()");
}

// Turn the AST into our internal representation
Result* parseAST(const mpc_ast_t* node) {
    switch (parseTag(node->tag)) {
        case TAG_INT: {
            long val = strtol(node->contents, NULL, 0);
            if (errno is ERANGE) { return errResult(INT_FLOW); }
            // We shouldn't need to check for other failures since the
            // language grammar ensures valid numbers only.
            return valResult(val);
        }

            // TODO: Make symbols first-class datatype
        case TAG_SYM: return symResult(node->contents);

        case TAG_SEXPR:;

            Result* sexpr = sexprResult();

            // We want to loop from 1 to children_num-1 since the first and last
            // children are going to be '(' and ')' respectively
            for (int i = 1; i < node->children_num - 1; i++) {
                resultListAppend(sexpr, parseAST(node->children[i]));
            }

            return sexpr;

        case TAG_QUOTE:;

            Result* quote = quoteResult();

            // We want to loop from 1 to children_num-1 since the first and last
            // children are going to be '{' and '}' respectively
            for (int i = 1; i < node->children_num - 1; i++) {
                resultListAppend(quote, parseAST(node->children[i]));
            }

            return quote;

        default: PANIC("Attempted to parse unknown tag");
    }
}

static void _printAST(const mpc_ast_t* const node, const int level) {
    for (int i = 0; i < level; i++) { printf("\t"); }
    printf("%s:%s\t-->(%d)\n", strcmp(node->tag, "") ? node->tag : "/",
           strcmp(node->contents, "") ? node->contents : "_",
           node->children_num);
    for (int i = 0; i < node->children_num; i++) {
        _printAST(node->children[i], level + 1);
    }
}
void printAST(const mpc_ast_t* const node) {
    _printAST(node, 0);
}
