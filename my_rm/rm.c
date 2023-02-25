#include <stdio.h>
#include <dirent.h>
#include "list.h"
#include <readline/readline.h>

// recursively iterate through the directory and its subdirectories and remove all files
int remove_dir(char *file, int vflag, int iflag)
{
    DIR *dir;
    struct dirent *directory;

    dir = opendir(file);

    int is_dir = 0;
    int contains_things = 0;

    if (dir)
    {
        is_dir = 1;

        while ((directory = readdir(dir)) != NULL)
        {
            if (directory->d_name[0] == '.')
                continue;
            else
            {
                if (iflag && contains_things == 0)
                {
                    fprintf(stderr, "rm: descend into directory '%s'? ", file);
                    printf("rm: descend into directory '%s'? ", file);
                    // char *c = readline("(y/n) ");
                    char c = getchar();
                    // flush the buffer
                    if (c != 'y')
                        return 0;
                }

                contains_things = 1;

                char *new_file = (char *)malloc(strlen(file) + strlen(directory->d_name) + 2);
                strcpy(new_file, file);
                strcat(new_file, "/");
                strcat(new_file, directory->d_name);

                remove_dir(new_file, vflag, iflag);
            }
        }
    }

    if (iflag)
    {
        if (is_dir)
        {
            fprintf(stderr, "rm: remove directory '%s'? ", file);
            char c = getchar();
            if (c != 'y')
                return 0;
        }
        else
        {
            fprintf(stderr, "rm: remove regular file '%s'? ", file);
            char c = getchar();
            if (c != 'y')
                return 0;
        }
    }

    if (remove(file) != 0)
        perror("Error");
    else if (vflag)
    {
        if (is_dir)
            printf("removed directory '%s'\n", file);
        else
            printf("removed '%s'\n", file);
    }

    return 0;
}

int main(int argc, char **argv)
{
    List files;
    init(&files);
    int iflag = 0, rflag = 0, vflag = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; j < strlen(argv[i]); j++)
            {
                if (argv[i][j] == 'i')
                    iflag = 1;
                else if (argv[i][j] == 'r' || argv[i][j] == 'R')
                    rflag = 1;
                else if (argv[i][j] == 'v')
                    vflag = 1;
            }
        }
        else
        {
            add(&files, argv[i]);
        }
    }

    for (int i = 0; i < files.size; i++)
    {
        DIR *dir;

        dir = opendir(files.content[i]);

        if (dir)
        {
            if (rflag)
            {
                remove_dir(files.content[i], vflag, iflag);
            }
            else
            {
                printf("rm: cannot remove '%s': Is a directory\n", files.content[i]);
            }
        }
        else
        {
            if (iflag)
            {
                printf("rm: remove regular file '%s'? ", files.content[i]);
                // char *c = readline("(y/n) ");
                char c = getchar();
                if (c != 'y')
                    continue;
            }

            if (remove(files.content[i]) != 0)
                perror("Error");
            else if (vflag)
                printf("removed '%s'\n", files.content[i]);
        }
    }

    return 0;
}