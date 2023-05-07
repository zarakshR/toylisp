#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

int main() {
    while (1) {
        char* readLine = readline("> ");
        if (!readLine) { break; }
        add_history(readLine);
        printf("%s\n", readLine);
        free(readLine);
    }
    return 0;
}

