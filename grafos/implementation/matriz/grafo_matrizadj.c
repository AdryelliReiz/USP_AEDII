#include <stdio.h>
#include "grafo_matrizadj.h"

// O(V²)
bool inicializaGrafo(Grafo* grafo, int nv) {
    int i, j;

    //verifica se o numero de vertices ultrapassa o max aceitavel de vertices
    if(nv > MAXNUMVERTICES) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: Número de vertices maior que o máximo permitido de %d.\n", MAXNUMVERTICES);
        return false;
    }

    //verifica se o numero de vertices é positivo
    if(nv <= 0) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: Número de vertices deve ser positivo.\n");
        return false;
    }

    //setta as informaçoes do grafo
    grafo->numVertices = nv;
    grafo->numArestas = 0;

    //percorre a matriz adjacente do grafo e adiciona os pesos das arestas
    for (i = 0; i < grafo->numVertices; i++) {
        for (j = 0; j < grafo->numVertices; j++) {
            grafo->mat[i][j] = AN;
        }
    }

    return true;
}

// O(V²)
void imprimeGrafo(Grafo* grafo, int nv) {
    if(v > grafo->numVertices) {
        fprintf(stderr, "ERRO: Número de vertices (%d) maior ou igual que o número total de vertices (%d).\n", v, grafo->numVertices);

        return;
    }

    int i, j;
    //percorre a matriz e imprime os valores do peso de cada aresta
    for(i = 0; i < nv; i++) {
        for(j = 0; j < nv; j++) {
            printf("%d ", grafo->mat[i][j]);
        }
    }
}

bool verificaValidadeVertice(int v, Grafo *grafo) {
    if(v > grafo->numVertices) {
        fprintf(stderr, "ERRO: Número de vertices (%d) maior ou igual que o número total de vertices (%d).\n", v, grafo->numVertices);

        return false;
    }

    if(v < 0) {
        fprintf(stderr, "ERRO: Número do vertice (%d) deve ser não negatico.\n", v);

        return false;
    }

    return true;
}

// O(1)
bool existeAresta(int v1, int v2, Grafo *grafo) {
    //verifica se os vertices são validos
    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return false;
    }

    if(grafo->mat[v1][v2] != AN) {
        return true;
    }

    return false;
}

// O(1)
void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
    //verifica se os vertices são validos
    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return;
    }

    grafo->mat[v1][v2] = peso;
    grafo->numArestas++;
}

// O(1)
Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    //verifica se os vertices são validos
    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return;
    }

    return grafo->mat[v1][v2];
}

// O(1)
bool removeAresta(int v1, int v2, Peso* peso, Grafo *grafo) {
    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return false;
    }

    //se aresta existe
    if(grafo->mat[v1][v2] != AN) {
        *peso = grafo->mat[v1][v2];
        grafo->mat[v1][v2] = AN;
        grafo->numArestas--;

        return true;
    }

    return false;
}

// O(V)
bool listaAdjVazia(int v, Grafo* grafo) {
    if(!verificaValidade(v, grafo)) {
        return true;
    }

    int i;
    for (i = 0; i< grafo->numVertices; i++) {
        if(grafo->mat[v][i] != AN) return false;
    }

    return true;
}

// O(V)
int proxListaAdj(int v, Grafo* grafo, int atual) {
    if(!verificaValidadeVertice(v, grafo)) {
        return VERTICE_INVALIDO;
    }

    atual++;
    while((atual < grafo->numVertices) && (grafo->mat[v][atual] == AN)) {
        atual ++;
    }

    if(atual > grafo->numVertices) {
        return VERTICE_INVALIDO;
    }

    return atual;
}

// O(1)
/*Não precisa fazer nada para matrizes de adjacencia*/
void liberaGrafo(Grafo* grafo) {
    free(grafo);

    return;
}
