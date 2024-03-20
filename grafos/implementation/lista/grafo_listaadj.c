#include <stdio.h>
#include <stdlib.h>

//leitura do header com estruturas e tipos especiais
#include "grafo_listaadj.h"

// O(V)
bool inicializaGrafo(TipoGrado* grafo, int nv) {
    //verifica se o grafo existe
    if(grafo == NULL) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo grafo == NULL.\n");
        return false;
    }

    //verifica se o valor é positivo
    if(nv <= 0) {
        fprintf(stderr, "Erro na chamada de inicializaGrafo: Número de vertices deve ser positivo.\n");
        return false;
    }

    //define o número de vertices do grafo
    grafo->numVertices = nv;
    //aloca memória para os vertices
    if(!(grafo->listaAdj = (Apontador*) calloc(nv, sizeof(Apontador)))) {
        fprintf(stderr, "Erro: Falha na alocação de memória na função inicializaGrafo.\n");
        return false;
    }
    
    //setta o número de arestas
    grafo->numAresta = 0;
    
    return true;
}

// O(1)
bool vericaValidadeVertice(int v, Grafo* grafo) {
    if(grafo->listaAdj[v] == NULL) 
        return true;

    return false;
}

// O(1)
bool listaAdjVazia(int v, Grafo* grafo) {
    if(!vericaValidadeVertice(v, grafo))
        return false;
    return (grafo->listaAdj[v] == NULL);
}

// O(1)
Apontador* proxListaAdj(int v Grafo* grafo, Aresta* atual) {
    if(atual == NULL) {
        fprintf(stderr, "atual == NULL.\n");
        return VERTICE_INVALIDO;
    }
    return (atual->prox;)
}

// O(1)
Apontador primeiroListaAdj(int v, Grafo* grafo) {
    return (grafo->listaAdj[v]);
}

// O(V)
bool existeAresta(int v1, int v2, Grafo* grafo) {
    Apontador q;

    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
        return false;
    
    q = grafo->listaAdj[v1];
    while((q != NULL) && (q->vdest != v2))
        q = q->prox;
    
    if(q != NULL) return true;
    return false;
}

// O(V)
Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
    Apontador q;

    if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
        return VERTICE_INVALIDO;

    q = grafo->listaAdj[v1];
    while((q != NULL) && (q->vdest != v2))
        q = q->prox;

    if(q != NULL) return VERTICE_INVALIDO;
    
    return q->peso;
}

// O(V)
void insereAresta(int v1, int v2, Peso peso, Grafo* grafo) {
    Apontador q = grafo->listaAdj[v1];
    Apontador* ant;

    while (q != NULL) {
       *ant = q;
       q = q->prox;
    }

    Apontador* novaAresta = (Apontador*) calloc(v2, sizeof(Apontador));
    novaAresta->peso = peso;
    novaAresta->prox = NULL;

    if(ant == NULL) {
        grafo->listaAdj[v1] = novaAresta;
    }  else {
        
        *ant->prox = novaAresta;
    }

    return;
}

/*
Função que insere uma resta no grafo (versão do slide)
*/
void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
    Apontador p;

    if(!(vericaValidadeVertice(v1, grafo) && vericaValidadeVertice(v2, grafo))) {
        fprintf(stderr, "Erro na chamada de insereAresta: Vértice(s) inválido(s).\n");
        return;
    }

    if(!(p = (Apontador) malloc(sizeof(Aresta)))) {
        fprintf(stderr, "Erro na chamada de insereAresta: Falha na alocação de memória.\n");
        return;
    }

    p->vdest = v2;
    p->peso = peso;
    p->prox = grafo->listaAdj[v1];
    grafo->listaAdj[v1] = p;
    grafo->numArestas++;
}

// O(V)
bool removeArestaObtendoPeso(int v1, int v2, Peso*) {
    Apontador q = grafo->listaAdj[v1];
    Apontador* ant;

    while ((q != NULL) && (q->vdest != v2)) {
       *ant = q;
       q = q->prox;
    }

    if((q != NULL) && q->vdest == v2) {
        if(ant != NULL) {
            *ant->prox = NULL;
        } else {
            grafo->listaAdj[v1] = NULL;
        }
        peso = q->peso;
        free(q);
        return true;
    }

    return false;
}

/*
Função que remove uma aresta do grafo (versão do slide)
*/
bool removeAresta(int v1, int v2, Grafo *grafo) {
    Apontador p, ant;

    if(!(vericaValidadeVertice(v1, grafo) && vericaValidadeVertice(v2, grafo))) {
        fprintf(stderr, "Erro na chamada de removeAresta: Vértice(s) inválido(s).\n");
        return false;
    }

    p = grafo->listaAdj[v1];
    ant = NULL;
    while((p != NULL) && (p->vdest != v2)) {
        ant = p;
        p = p->prox;
    }

    if(q != NULL) {
        if(grafo->listaAdj[v1] == p) {
            grafo->listaAdj[v1] = p->prox;
        } else {
            ant->prox = p->prox;
        }
        *peso = p->peso;
        q->prox = NULL;
        free(q);
        q = NULL;
        return true;
    }

    return false;
}

// O(V)
void imprimeGrafo(Grafo* grafo) {
    Apontador q;

    int i;
    for (i = 0; i < grafo->numVertices; i++) {
        q = grafo->listaAdj[i];

        while(q != NULL) {
            printf("%d ----- %d\n", q->peso, q->vdest);
            q = q->prox;
        }
    }

    return;
}

int leGrafo(char* nomearq, Grafo* grafo) {
    FILE *fp;
    int nvertices, narestas;
    int v1, v2;
    Peso peso;
    int idar;

    fp = fopen(nomearq, "r");
    if(fp == NULL) return (0);

    if(fscanf(fp, "%d %d", &nvertices, &narestas) != 2) return (0);

    inicializaGrafo(grafo, nvertices);
}

void liberaGrafo(Grafo* grafo) {
    Apontador p;
    int v;

    for(v = 0; v < grafo->numVertices; v++) {
        while((p = grafo->listaAdj[v]) != NULL) {
            grafo->listaAdj[v] = p->prox;
            p->prox = NULL;
            free(p);
        }
    }

    grafo->numVertices = 0;
    free(grafo->listaAdj);
    grafo->listaAdj = NULL;
}