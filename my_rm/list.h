#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>

struct List
{
    char **content;
    int size;
};

typedef struct List List;

//initialize the list
void init(List *list)
{
    list->content = NULL;
    list->size = 0;
}

//adds a string to the list
void add(List *list, char *string)
{
    char **new_content = (char **)malloc((list->size + 1) * sizeof(char *));

    for (int i = 0; i < list->size; i++)
    {
        new_content[i] = (char *)malloc(strlen(list->content[i]) + 1);
        strcpy(new_content[i], list->content[i]);
    }

    new_content[list->size] = (char *)malloc(strlen(string) + 1);
    strcpy(new_content[list->size], string);

    if (list->content != NULL)
    {
        free(list->content);
    }

    list->size++;
    list->content = new_content;
}

#endif