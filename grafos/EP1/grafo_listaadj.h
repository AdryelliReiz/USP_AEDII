#include <stdbool.h>

#define VERTICE_INVALIDO NULL
#define AN -1

typedef int Peso;
typedef struct str_aresta {
    int vdest;
    Peso peso;
    struct str_aresta * prox;
} Aresta;
typedef Aresta* Apontador;
typedef struct {
    Apontador* listaAdj;
    int numVertices;
    int numArestas;
} Grafo;