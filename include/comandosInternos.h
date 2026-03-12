#ifndef COMANDOS_INTERNOS_H
#define COMANDOS_INTERNOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "struct.h"
#include <fcntl.h>

int tratar_comandos_internos(char *args[], mesoShellEstilo *modo);
pid_t executar_comando(char *args[], mesoShellEstilo modo_atual, int background);
void rodar_modo_interativo();
void rodar_modo_arquivo(const char *nome_arquivo);
int verificar_background(char *args[]);

#endif 