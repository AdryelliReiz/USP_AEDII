#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "grafo_listaadj.h"

void inicializaGrafo(Grafo *grafo, int numVertices) {
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    grafo->listaAdj = (Apontador*) malloc(numVertices * sizeof(Apontador));
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
    //aqui temos certeza que o v1 e v2 existem, ok?
    Apontador q = grafo->listaAdj[v1];
    while((q != NULL) && (q->vdest != v2))
        q = q->prox;

    if(q != NULL) {
        //realmente existe a aresta? confirma para mim (pq não é direcionado)
        q = grafo->listaAdj[v2];
        while((q != NULL) && (q->vdest != v1))
            q = q->prox;

        if(q != NULL) {
            return true;
        }
    }
    return false;
}

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
    if(!verificaValidadeVertice(v1, grafo)) return;
    if(!verificaValidadeVertice(v2, grafo)) return;

    if(!existeAresta(v1, v2, grafo)) {
        //cria nova aresta para v2 e inserir no final de v1
        Apontador novaArestaV1 = (Apontador) malloc(sizeof(Aresta));
        novaArestaV1->vdest = v2;
        novaArestaV1->peso = peso;
        novaArestaV1->prox = NULL;

        if (grafo->listaAdj[v1] == NULL) {
            grafo->listaAdj[v1] = novaArestaV1;
        } else {
            Apontador p = grafo->listaAdj[v1];
            while (p->prox != NULL) {
                p = p->prox;
            }
            p->prox = novaArestaV1;
        }

        //cria nova aresta para v1 e inserir no final de v2
        Apontador novaArestaV2 = (Apontador) malloc(sizeof(Aresta));
        novaArestaV2->vdest = v1;
        novaArestaV2->peso = peso;
        novaArestaV2->prox = NULL;

        if (grafo->listaAdj[v2] == NULL) {
            grafo->listaAdj[v2] = novaArestaV2;
        } else {
            Apontador p = grafo->listaAdj[v2];
            while (p->prox != NULL) {
                p = p->prox;
            }
            p->prox = novaArestaV2;
        }
        grafo->numArestas++;
    }
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    Apontador q = grafo->listaAdj[v1];
    while((q != NULL) && (q->vdest != v2))
        q = q->prox;

    if(q != NULL) {
        return q->peso;
    }
    return AN;
}

bool removeAresta(int v1, int v2, Grafo *grafo) {
    if(existeAresta(v1, v2, grafo)) {
        Apontador q = grafo->listaAdj[v1];
        Apontador ant = NULL;

        while((q != NULL) && (q->vdest != v2)) {
            ant = q;
            q = q->prox;
        }
        
        //ajusta os ponteiros para o próximo vertice
        if(ant == NULL) {
            grafo->listaAdj[v1] = q->prox;
        } else {
            ant->prox = q->prox;
        }

        free(q);

        //faz o mesmo para o vertice v2
        q = grafo->listaAdj[v2];
        ant = NULL;

        while((q != NULL) && (q->vdest != v1)) {
            ant = q;
            q = q->prox;
        }

        if(ant == NULL) {
            grafo->listaAdj[v2] = q->prox;
        } else {
            ant->prox = q->prox;
        }

        free(q);

        return true;
    }
    return false;
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo) {
    *peso = obtemPesoAresta(v1, v2, grafo);

    if(removeAresta(v1, v2, grafo)) {
        return true;
    }
    return false;
}

bool listaAdjVazia(int v, Grafo* grafo) {
    return grafo->listaAdj[v] == NULL;
}

Apontador primeiroListaAdj(int v, Grafo* grafo) {
    if(!verificaValidadeVertice(v, grafo)) return NULL;

    return grafo->listaAdj[v];
}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual) {
    if(!verificaValidadeVertice(v, grafo) || atual == NULL ||!verificaValidadeVertice(atual->vdest, grafo)) return NULL;
    if(!existeAresta(v, atual->vdest, grafo)) return NULL;

    return atual->prox;
}

int obtemVerticeDestino(Apontador p, Grafo* grafo) {
    if(p == NULL) return -1;
    if(!verificaValidadeVertice(p->vdest, grafo)) return -1;
    return p->vdest;
}
//ta com erro
void liberaGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        Apontador atual = grafo->listaAdj[i];
        while (atual != NULL) {
            Apontador proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(grafo);
}

void imprimeGrafo(Grafo *grafo) {
    for(int i = 0; i < grafo->numVertices; i++){
        Apontador p = grafo->listaAdj[i];
        if(p == NULL){
            printf("%d <-------> %s\n", i, "Nenhum vértice adjacente");
        } else {
            while(p != NULL){
                printf("%d <-------> %d | peso: %d\n", i, p->vdest, p->peso);
                p = p->prox;
            }
        }
    }
}

bool verificaApontador(Apontador p, Grafo* grafo) {
    return p != NULL;
}