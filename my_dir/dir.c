#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    DIR* dir;
    struct dirent* directory;

    dir = opendir(getcwd(NULL, 0));

    if(argc == 2) {
        dir = opendir(argv[1]);
    }
    else {
        dir = opendir(getcwd(NULL, 0));
    }

    if(dir) {
        while((directory = readdir(dir)) != NULL){
            if(directory->d_name[0] == '.') 
                continue;
            else {
                printf("%s\n", directory->d_name);
                fflush(stdout);
            }
        }
    }
    else {
        printf("Not a directory.");
    }

    return 0;
}