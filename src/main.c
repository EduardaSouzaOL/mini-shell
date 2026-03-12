#include <stdio.h>
#include "comandosInternos.h"


int main(int argc, char *argv[]) {
    if (argc == 1) {
        rodar_modo_interativo();
    } 
    else if (argc == 2) {
        rodar_modo_arquivo(argv[1]);
    } 
    else {
        printf("ERRO: Número de argumentos inválido! Use: %s <arquivo>\n", argv[0]);
        return 1;
    }
    
    return 0;
}
