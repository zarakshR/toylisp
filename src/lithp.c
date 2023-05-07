#include <stdio.h>
#include <stdlib.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "mpc.h"
#include "parser.h"

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
