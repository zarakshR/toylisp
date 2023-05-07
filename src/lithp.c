#include <stdio.h>
#include <stdlib.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "mpc.h"

// Parsers
mpc_parser_t* Number;
mpc_parser_t* Operator;
mpc_parser_t* Expr;
mpc_parser_t* Lispy;

void setup_parser(void) {
    // Initialize Parsers
    Number   = mpc_new("number");
    Operator = mpc_new("operator");
    Expr     = mpc_new("expr");
    Lispy    = mpc_new("lispy");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
        number   : /-?[0-9]+/ ;                             \
        operator : '+' | '-' | '*' | '/' ;                  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;  \
        lispy    : /^/ <operator> <expr>+ /$/ ;             \
      ",
              Number, Operator, Expr, Lispy);
}

void cleanup_parser(void) {
    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, Lispy);
}

int main() {
    setup_parser();

    while (1) {
        mpc_result_t r;

        char* readLine = readline("> ");
        if (!readLine) { break; }
        add_history(readLine);

        if (mpc_parse("<stdin>", readLine, Lispy, &r)) {
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(readLine);
    }

    cleanup_parser();

    return 0;
}
