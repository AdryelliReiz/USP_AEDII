#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Número de argumentos inválido! É necessário fornecer o nome do arquivo de test e q chaves :/\n");
        return 1;
    }
    //criando arquiv de saida
    char exitFilename[100] = "";
    strcpy(exitFilename, argv[1]);
    FILE *efp = fopen(exitFilename, "w");

    printf("Arquivo de saída criado com sucesso!\n");

    int chaves = atoi(argv[2]);

    for(int i = 0; i < chaves; i++) {
        fprintf(efp, "i %d\np\n", i);
    }

    fclose(efp);

    return 0;
}