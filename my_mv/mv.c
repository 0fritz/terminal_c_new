#include <stdio.h>
#include <string.h>
#include "list.h"
#include <readline/readline.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Error: Missing arguments.\n");
        printf("Usage: mv <source> <destination>\n");
        return -1;
    }

    int iflag = 0, tflag = 0, sflag = 0, bflag = 0;

    char *dest = NULL;
    List src;
    init(&src);

    char *suffix = "~";

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; j < strlen(argv[i]); j++)
            {
                if (argv[i][j] == 'i')
                    iflag = 1;
                else if (argv[i][j] == 't')
                {
                    tflag = 1;

                    if (i + 1 < argc)
                    {
                        if (dest != NULL)
                            add(&src, dest);

                        dest = argv[i + 1];
                    }
                    else
                    {
                        printf("Error: Missing argument for flag -t.\n");
                        printf("Usage: mv <sources> -t <destination>\n");
                        return -1;
                    }
                }
                else if (argv[i][j] == 's')
                {
                    sflag = 1;

                    if (i + 1 < argc)
                    {
                        suffix = argv[i + 1];
                    }
                    else
                    {
                        printf("Error: Missing argument for flag -s.\n");
                        printf("Usage: mv <sources> -s <suffix>\n");
                        return -1;
                    }
                }
                else if (argv[i][j] == 'b')
                    bflag = 1;
                else
                {
                    printf("Error: Unknown option %c\n", argv[i][j]);
                    printf("Usage: mv <source> <destination>\n");
                    return -1;
                }
            }
        }
        else
        {
            if ((dest != NULL && strcmp(dest, argv[i]) == 0) || (suffix != NULL && strcmp(suffix, argv[i]) == 0))
                continue;

            // if there are no sources, set first non-flag argument as source
            if (src.size == 0)
                add(&src, argv[i]);
            // if there is a source but no destination, set second non-flag argument as destination
            else if (dest == NULL)
                dest = argv[i];
            // if there is a source and a destination, add the rest of the arguments to the list of sources
            else
                add(&src, argv[i]);
        }
    }

    // "mv x y z" shouldn't work, but "mv x y -t z" should
    if (src.size > 1 && !tflag)
    {
        printf("Error: There can only be a single source.\n");
        printf("Usage: mv <source> <destination>\n");
        return -1;
    }

    for (int i = 0; i < src.size; i++)
    {
        char *new_dest = dest;

        if (tflag == 1)
        {
            new_dest = malloc(strlen(dest) + strlen(src.content[i]) + 2);
            strcpy(new_dest, dest);
            strcat(new_dest, "/");
            strcat(new_dest, src.content[i]);
        }

        FILE *file;
        if (iflag == 1 && (file = fopen(new_dest, "r+")))
        {
            fclose(file);

            printf("Overwrite %s? ", new_dest);
            char *input = readline("(y/n) ");

            if (input[0] != 'y')
                continue;
        }

        if (bflag == 1)
        {
            char *backup_dest = malloc(strlen(new_dest) + strlen(suffix) + 1);
            strcpy(backup_dest, new_dest);
            strcat(backup_dest, suffix);

            if (rename(new_dest, backup_dest) != 0)
            {
                perror("Error");
                printf("Usage: mv <source> <destination>\n");
                return -1;
            }
        }

        if (rename(src.content[i], new_dest) != 0)
        {
            perror("Error");
            printf("Usage: mv <source> <destination>\n");
            return -1;
        }
    }
}