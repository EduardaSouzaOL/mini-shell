#include "comandosInternos.h"
#include "controleProcessos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
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
            *modo = MODO_PARALELO;
        else if (args[1] != NULL && (strcmp(args[1], "sequencial") == 0 || strcmp(args[1], "sequential") == 0))

            *modo = MODO_SEQUENCIAL;
        return 1;
    }

    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            char *home = getenv("HOME");
            if (home != NULL)
                chdir(home);
            else
                printf("mesoShell: cd: variável HOME não encontrada\n");
        }
        else
        {
            if (chdir(args[1]) != 0)
                perror("mesoShell: cd");
        }
        return 1;
    }

    if (strcmp(args[0], "fg") == 0)
    {
        if (args[1] == NULL)
        {
            printf("mesoShell: fg: argumento requerido (ex: fg 1)\n");
        }
        else
        {
            int id = atoi(args[1]);
            if (!fg_job(id))
            {
                printf("mesoShell: fg: job [%d] não existe ou já foi finalizado.\n", id);
            }
        }
        return 1;
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
        tratar_redirecionamento(args);
        if (execvp(args[0], args) == -1)
        {
            perror("mesoShell: comando não encontrado");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        if (modo_atual == MODO_SEQUENCIAL && background == 0)
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
        i++;

    if (i > 0 && strcmp(args[i - 1], "&") == 0)
    {
        args[i - 1] = NULL;
        return 1;
    }
    return 0;
}

void rodar_modo_interativo()
{
    char linha_imput[1024];
    char *comandos[100];
    int total_comandos = 0;
    char *args[100];
    mesoShellEstilo modo_padrao = MODO_SEQUENCIAL;

    init_jobs();

    while (1)
    {
        check_jobs();

        printf(modo_padrao == MODO_SEQUENCIAL ? "mesoShell seq> " : "mesoShell par> ");

        if (fgets(linha_imput, sizeof(linha_imput), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        linha_imput[strcspn(linha_imput, "\n")] = '\0';

        dividir_comandos(linha_imput, comandos, &total_comandos);

        pid_t pids_ativos[100];
        int qtd_processos = 0;

        for (int i = 0; i < total_comandos; i++)
        {
            separar_comando(comandos[i], args);

            int background = verificar_background(args);

            int status_interno = tratar_comandos_internos(args, &modo_padrao);

            if (status_interno == -1)
                return;
            if (status_interno == 1)
                continue;

            pid_t pid = executar_comando(args, modo_padrao, background);

            if (pid > 0)
            {
                if (background)
                {
                    add_job(pid);
                }
                else
                {
                    pids_ativos[qtd_processos++] = pid;
                }
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

    init_jobs();

    while (fgets(linha_imput, sizeof(linha_imput), arquivo))
    {
        check_jobs();

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
                if (background)
                {
                    add_job(pid);
                }
                else
                {
                    pids_ativos[qtd_processos++] = pid;
                }
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