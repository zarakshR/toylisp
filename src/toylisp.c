#include "toylisp.h"

int main() {
    setup_parser();

    loop {
        mpc_result_t r;

        char* readLine = readline("> ");
        if (readLine is NULL) { break; }
        add_history(readLine);

        if (mpc_parse("<stdin>", readLine, Program, &r)) {

            // We need to explicitly cast r.output since it is a void*
            mpc_ast_t* parser_output = (mpc_ast_t*)r.output;

            // Since the parser effectively wraps every line in an expression
            // block, we need to loop through each child of the outermost
            // expression and evalute each element separately.
            for (int i = 1; i < parser_output->children_num - 1; i++) {
                Result* program     = parseAST(parser_output->children[i]);
                Result* eval_result = eval(program);
                printResult(eval_result);
                resultFree(eval_result);
            }
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
