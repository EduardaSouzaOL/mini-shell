#ifndef STRINGS_H
#define STRINGS_H

char *separar_comando(char *linha, char *args[]);
void print_args(char *args[]);
void dividir_comandos(char *linha, char *comandos[], int *total) ;
#endif