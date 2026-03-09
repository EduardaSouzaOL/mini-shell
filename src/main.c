#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // Para o fork() e execvp()
#include <sys/wait.h>   // Para o waitpid()

int main(int argc, char *argv[])
{
    char linha[1024];
    char *args[100];

    if (argc == 1)
    {
        // modo interativo
        while (1)
        {
            printf("mesoShell seq> ");

            if (fgets(linha, sizeof(linha), stdin) == NULL)
            {
                printf("\nERRO: EOF - end of file / CTRL+D\n");
                break;
            }

            if (strcmp(linha, "exit\n") == 0)
            {
                break;
            }

            separar_comando(linha, args);
            print_args(args);
            
        }
    }
    else if (argc == 2)
    {
        FILE *arquivo = fopen(argv[1], "r");

        if (arquivo == NULL)
        {
            printf("Erro ao abrir arquivo\n");
            return 1;
        }

        while (fgets(linha, sizeof(linha), arquivo))
        {
            printf("%s", linha);
        }

        fclose(arquivo);
    }
    else
    {
        printf("ERRO: Número de argumentos inválido. Use: %s <arquivo>\n", argv[0]);
    }

    return 0;
}