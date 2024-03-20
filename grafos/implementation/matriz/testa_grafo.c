#include <stdio.h>
#include "grafo_matrizadj.h"

int main() {
    Grafo g1;
    int numVertices;

    inicializaGrafo(&g1, 10);

    /*do {
        printf("Digite o número de vértices do grafo:\n");
        scanf("%d", &numVertices);
    } while (!inicializaGrafo(&g1, numVertices));
*/
    //imprimeGrafo(&g1);

    return 0;
}