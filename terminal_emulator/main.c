#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "run_command.h"

int main(int argc, char **argv)
{
    using_history();

    char *cur_dir = getenv("HOME");
    chdir(cur_dir);

    while (1)
    {
        char *input = readline(">");

        char *output = run_command(input);

        printf("%s", output);

        free(input);
        free(output);
    }
}