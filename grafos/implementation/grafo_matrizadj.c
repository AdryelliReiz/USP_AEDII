#include <stdio.h>
#include "grafo_matrizadj.h"

bool inicializaGrafo(Grafo* grafo, int nv) {
    int i, j;

    if(nv > MAXNUMVERTICES) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: Número de vertices maior que o máximo permitido de %d.\n", MAXNUMVERTICES);
        return false;
    }

    if(nv <= 0) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: Número de vertices deve ser positivo.\n");
        return false;
    }

    grafo->numVertices = nv;
    grafo->numArestas = 0;

    for (i = 0; i < grafo->numVertices; i++) {
        for (j = 0; j < grafo->numVertices; j++) {
            grafo->mat[i][j] = AN;
        }
    }

    return true;
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

bool existeAresta(int v1, int v2, Grafo *grafo) {
    //Exercício
}

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return;
    }

    grafo->mat[v1][v2] = peso;
    grafo->numArestas++;
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    //Exercício
}

bool removeAresta(int v1, int v2, Peso* peso, Grafo *grafo) {
    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo))) {
        return false;
    }

    if(grafo->mat[v1][v2] != AN) {
        *peso = grafo->mat[v1][v2];
        grafo->mat[v1][v2] = AN;
        grafo->numArestas--;

        return true;
    }

    return false;
}

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

/*Não precisa fazer nada para matrizes de adjacencia*/
void liberaGrafo(Grafo* grafo) {}
