#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "grafo_matrizadj.h"

void inicializaGrafo(Grafo *grafo, int numVertices) {
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    for(int i = 0; i < MAXNUMVERTICES; i++){
        for(int j = 0; j < MAXNUMVERTICES; j++){
            grafo->mat[i][j] = 0;
        }
    }
}

int obtemNrVertices(Grafo* grafo) {
    return grafo->numVertices;
}

int obtemNrArestas(Grafo* grafo) {
    return grafo->numArestas;
}

bool verificaValidadeVertice(int v, Grafo *grafo) {
    return v >= 0 && v < grafo->numVertices;
}

bool existeAresta(int v1, int v2, Grafo *grafo) {
    //aqui temos certeza que o v1 e o v2 existem, ok?
    return grafo->mat[v1][v2] != 0;
}

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
    if(!verificaValidadeVertice(v1, grafo)) return;
    if(!verificaValidadeVertice(v2, grafo)) return;

    if(!existeAresta(v1, v2, grafo)){
        grafo->mat[v1][v2] = peso;
        grafo->mat[v2][v1] = peso;
        grafo->numArestas++;
    }
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    return grafo->mat[v1][v2];
}

bool removeAresta(int v1, int v2, Grafo *grafo) {
    if(existeAresta(v1, v2, grafo)){
        grafo->mat[v1][v2] = 0;
        grafo->mat[v2][v1] = 0;
        grafo->numArestas--;
        return true;
    }
    return false;
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo) {
    *peso = obtemPesoAresta(v1, v2, grafo);

    if(removeAresta(v1, v2, grafo)){
        return true;
    }
    return false;
}

bool listaAdjVazia(int v, Grafo* grafo) {
    for(int i = 0; i < grafo->numVertices; i++){
        if(existeAresta(v, i, grafo)){
            return false;
        }
    }
    return true;
}

Apontador primeiroListaAdj(int v, Grafo* grafo) {
    if(!verificaValidadeVertice(v, grafo)) return -1; 
    for(int i = 0; i < grafo->numVertices; i++) {
        if(existeAresta(v, i, grafo)){
            return i;
        }
    }
    return -1;
}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual) {
    if (!verificaValidadeVertice(v, grafo) || !verificaValidadeVertice(atual, grafo)) return -1;

    int i = atual + 1;
    while (i < grafo->numVertices) {
        if (existeAresta(v, i, grafo)) {
            return i;
        }
        i++;
    }

    return -1;
}

int obtemVerticeDestino(Apontador p, Grafo* grafo) {
    if(!verificaValidadeVertice(p, grafo)) return -1;
    
    return p;
}

//tá com erro
void liberaGrafo(Grafo* grafo) {}

void imprimeGrafo(Grafo *grafo) {
    for(int i = 0; i < grafo->numVertices; i++){
        int qVerticesAdj = 0;
        for(int j = 0; j < grafo->numVertices; j++){
            if(existeAresta(i, j, grafo)){
                qVerticesAdj++;
                printf("%d <-------> %d | peso: %d\n", i, j, grafo->mat[i][j]);
            }
        }

        //não achou nenhum vertice adjacente? imprime apenas o vertice
        if(qVerticesAdj == 0){
            printf("%d <-------> %s\n", i, "Nenhum vértice adjacente");
        }
    }
}

bool verificaApontador(Apontador p, Grafo* grafo) {
    return verificaValidadeVertice(p, grafo);
}