#include "comandosInternos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int tratar_comandos_internos(char *args[], mesoShellEstilo *modo)
{
    if (args[0] == NULL)
    {
        return 1;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        return -1;
    }

    if (strcmp(args[0], "style") == 0)
    {
        if (args[1] != NULL && (strcmp(args[1], "paralel") == 0 || strcmp(args[1], "paralelo") == 0 || strcmp(args[1], "parallel") == 0))
        {
            *modo = MODO_PARALELO;
        }
        else if (args[1] != NULL && (strcmp(args[1], "sequencial") == 0 || strcmp(args[1], "sequential") == 0))
        {
            *modo = MODO_SEQUENCIAL;
        }
        return 1;
    }
    if (strcmp(args[0], "cd") == 0)
    {
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                // sem argumento, ir para a pasta home
                char *home = getenv("HOME");
                if (home != NULL)
                {
                    chdir(home);
                }
                else
                {
                    printf("mesoShell: cd: variável HOME não encontrada\n");
                }
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("mesoShell: cd");
                }
            }
            return 1;
        }
    }

    return 0;
}

pid_t executar_comando(char *args[], mesoShellEstilo modo_atual, int background)
{
    if (args[0] == NULL)
        return -1;

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("mesoShell: erro no fork");
    }
    else if (pid == 0)
    {
        // Processo Filho
        if (execvp(args[0], args) == -1)
        {
            perror("mesoShell: comando não encontrado");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        // Processo Pai
        if (modo_atual == MODO_SEQUENCIAL)
        {
            int status;
            waitpid(pid, &status, 0);
        }
        return pid;
    }
    return -1;
}

int verificar_background(char *args[])
{
    int i = 0;
    while (args[i] != NULL)
    {
        i++;
    }
    if (i > 0 && strcmp(args[i - 1], "&") == 0)
    {
        args[i - 1] = NULL; // Remove o '&' dos argumentos
        return 1;           // Indica que é um comando em background
    }
    return 0; // Não é um comando em background
}

void rodar_modo_interativo()
{
    char linha_imput[1024];
    char *comandos[100];
    int total_comandos = 0;
    char *args[100];
    mesoShellEstilo modo_padrao = MODO_SEQUENCIAL;

    while (1)
    {
        printf(modo_padrao == MODO_SEQUENCIAL ? "mesoShell seq> " : "mesoShell par> ");

        if (fgets(linha_imput, sizeof(linha_imput), stdin) == NULL)
        {
            printf("\nERRO: EOF - end of file / CTRL+D\n");
            break;
        }

        linha_imput[strcspn(linha_imput, "\n")] = '\0';

        dividir_comandos(linha_imput, comandos, &total_comandos);

        pid_t pids_ativos[100];
        int qtd_processos = 0;
        int background = verificar_background(args);

        for (int i = 0; i < total_comandos; i++)
        {
            separar_comando(comandos[i], args);

            int background = verificar_background(args);

            int status_interno = tratar_comandos_internos(args, &modo_padrao);
            if (status_interno == -1)
                return; // Encerra o shell
            if (status_interno == 1)
                continue; // pula para o próximo comando

            pid_t pid = executar_comando(args, modo_padrao, background);

            
            if (pid > 0 && background == 0)
            {
                pids_ativos[qtd_processos++] = pid;
            }
        }
    }
}

    void rodar_modo_arquivo(const char *nome_arquivo)
    {
        char linha_imput[1024];
        char *comandos[100];
        int total_comandos = 0;
        char *args[100];
        mesoShellEstilo modo_padrao = MODO_SEQUENCIAL;

        FILE *arquivo = fopen(nome_arquivo, "r");
        if (arquivo == NULL)
        {
            printf("Erro ao abrir arquivo\n");
            return;
        }

        while (fgets(linha_imput, sizeof(linha_imput), arquivo))
        {
            linha_imput[strcspn(linha_imput, "\n")] = '\0';

            printf("Executando do arquivo: %s\n", linha_imput);

            dividir_comandos(linha_imput, comandos, &total_comandos);

            pid_t pids_ativos[100];
            int qtd_processos = 0;

            for (int i = 0; i < total_comandos; i++)
            {
                separar_comando(comandos[i], args);
                int background = verificar_background(args);
                int status_interno = tratar_comandos_internos(args, &modo_padrao);
                if (status_interno == -1)
                {
                    fclose(arquivo);
                    return;
                }
                if (status_interno == 1)
                    continue;

                pid_t pid = executar_comando(args, modo_padrao, background);
                if (pid > 0)
                {
                    pids_ativos[qtd_processos++] = pid;
                }
            }

            if (modo_padrao == MODO_PARALELO)
            {
                for (int i = 0; i < qtd_processos; i++)
                {
                    waitpid(pids_ativos[i], NULL, 0);
                }
            }
        }

        fclose(arquivo);
    }