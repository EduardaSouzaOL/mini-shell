#include <string.h>
#include "strings.h"
#include <stdio.h>

char *separar_comando(char *linha, char *args[])
{
    int i = 0;

    args[i] = strtok(linha," \t\n");

    while (args[i] != NULL)
    {
        i++;
        args[i] = strtok(NULL," \t\n");
    } 

    return args[0];
}

void print_args(char *args[])
{
    int i = 0;

    while (args[i] != NULL)
    {
        printf("Argumento %d: %s\n", i, args[i]);
        i++;
    }
}
