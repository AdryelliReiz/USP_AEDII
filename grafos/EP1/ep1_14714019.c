#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#ifdef MATRIZ
#include "grafo_matrizadj.h"
#else
#include "grafo_listaadj.h"
#endif

//Funções independentes

void imprimeGrafo(Grafo *grafo);
void inicializaGrafo(Grafo *grafo, int numVertices);
int obtemNrVertices(Grafo* grafo);
int obtemNrArestas(Grafo* grafo);
bool verificaValidadeVertice(Apontador v, Grafo *grafo);
void insereAresta(int v1, int v2, Peso peso, Grafo *grafo);
bool existeAresta(int v1, int v2, Grafo *grafo);
Peso obtemPesoAresta(int v1, int v2, Grafo *grafo);
bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo);
bool removeAresta(int v1, int v2, Grafo *grafo);
bool listaAdjVazia(int v, Grafo* grafo);
Apontador primeiroListaAdj(int v, Grafo* grafo);
Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual);
int obtemVerticeDestino(Apontador p, Grafo* grafo);
void imprimeGrafo(Grafo* grafo);
void liberaGrafo(Grafo* grafo);
//funções extras
int obtemValorVertice(Apontador p, Grafo* grafo);
bool verificaApontador(Apontador p, Grafo* grafo);

//Funções auxiliares

//função recursiva de visita em profundidade
void visitaEmLargura(int vertice, int visitados[], int nivel[], int antecessor[], int fila[], int *inicio, int *fim, Grafo *grafo) {
    int verticeAdj;
    Apontador apontadorVerticeAdj = primeiroListaAdj(vertice, grafo);
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1; //vertice invalido
    else verticeAdj = obtemValorVertice(apontadorVerticeAdj, grafo);

    while(verticeAdj != -1) {
        //coloca o vertices validos na fila
        if(!visitados[verticeAdj]) {
            visitados[verticeAdj] = 1;
            nivel[verticeAdj] = nivel[vertice] + 1;
            antecessor[verticeAdj] = vertice;
            fila[(*fim)++] = verticeAdj;
        }

        //atualiza o vertice a ser verificado
        apontadorVerticeAdj = proxListaAdj(vertice, grafo, apontadorVerticeAdj);
        if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
        else verticeAdj = obtemValorVertice(apontadorVerticeAdj, grafo);
    }
    //marca que visitou todos os adjacentes
    visitados[vertice] = 2;

    //se tem elementos na fila, continuar com a visita
    if(*inicio < *fim && (*fim) < obtemNrVertices(grafo)) {
        (*inicio)++;
        visitaEmLargura(fila[*inicio], visitados, nivel, antecessor, fila, inicio, fim, grafo);
    }
}

void visitaEmProfundidade(int vertice, int visitados[], int nivel[], int antecessor[], int fila[], int *fim, Grafo *grafo) {
    int verticeAdj;
    Apontador apontadorVerticeAdj = primeiroListaAdj(vertice, grafo);
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
    else verticeAdj = obtemValorVertice(apontadorVerticeAdj, grafo);

    // enquanto o vértice é válido, se não foi visitado-> visita, senão procura outro adjacente
    while(verticeAdj != -1) {
        if(!visitados[verticeAdj]) {
            visitados[verticeAdj] = 1;
            nivel[verticeAdj] = nivel[vertice] + 1;
            antecessor[verticeAdj] = vertice;
            fila[(*fim)++] = verticeAdj;
            visitaEmProfundidade(verticeAdj, visitados, nivel, antecessor, fila, fim, grafo);
            visitados[verticeAdj] = 2;
        }
        apontadorVerticeAdj = proxListaAdj(vertice, grafo, apontadorVerticeAdj);
        if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
        else verticeAdj = obtemValorVertice(apontadorVerticeAdj, grafo);
    }
}

void visitaEmProfundidadeComponentesConexos(int vertice, int visitados[], int antecessor[], int fila[], int *fim, Grafo *grafo) {
    int verticeAdj;
    Apontador apontadorVerticeAdj = primeiroListaAdj(vertice, grafo);
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
    else verticeAdj = obtemValorVertice(apontadorVerticeAdj, grafo);
    
    // enquanto o vértice é válido, se não foi visitado-> visita, senão procura outro adjacente
    while(verticeAdj != -1) {
        if(!visitados[verticeAdj]) {
            visitados[verticeAdj] = 1;
            antecessor[verticeAdj] = vertice;
            fila[(*fim)++] = verticeAdj;
            visitaEmProfundidadeComponentesConexos(verticeAdj, visitados, antecessor, fila, fim, grafo);
            visitados[verticeAdj] = 2;
        }
        apontadorVerticeAdj = proxListaAdj(vertice, grafo, apontadorVerticeAdj);
        if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
        else verticeAdj = obtemValorVertice(apontadorVerticeAdj, grafo);
    }
}

void ordenarArray(int array[], int n) {
    int i, j, aux;
    for(i = 0; i < n; i++) {
        for(j = i + 1; j < n; j++) {
            if(array[i] > array[j]) {
                aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }
}

//Funções principais que usam as funções acima '0'

//tá funfando legal
void buscaEmLargura(Grafo *grafo) {
    int visitados[obtemNrVertices(grafo)];
    int fila[obtemNrVertices(grafo)];
    int inicio = 0;
    int fim = 0;
    int nivel[obtemNrVertices(grafo)];
    int antecessor[obtemNrVertices(grafo)];
    int caminho[obtemNrVertices(grafo)][obtemNrVertices(grafo)];

    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        visitados[i] = 0;
        nivel[i] = -1;
        antecessor[i] = -1;
        for (int j = 0; j < obtemNrVertices(grafo); j++) {
            caminho[i][j] = -1;
        }
    }

    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        if(!visitados[i]) {
            visitados[i] = 1;
            nivel[i] = 0;
            antecessor[i] = -1;
            fila[fim++] = i;
            
            visitaEmLargura(i, visitados, nivel, antecessor, fila, &inicio, &fim, grafo);

            visitados[i] = 2; //marca como preto pq acabou os vertices adjacentes
        }
    }

    printf("BL:\n");
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        printf("%d ", fila[i]);
    }
    
    //monta os caminhos de cada vertice para o vertice inicial (antecessor)
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        int v = i;
        while (v != -1) {
            caminho[i][nivel[v]] = v;
            v = antecessor[v];
        }
    }

    printf("\n\n");
    printf("Caminhos BL:\n");
    //imprime caminhos
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        for (int j = 0; j < obtemNrVertices(grafo); j++) {
            if (caminho[i][j] != -1) {
                printf("%d ", caminho[i][j]);
            }
        }
        printf("\n");
    }
}

void buscaEmProfundidade(Grafo *grafo) {
    int antecessor[obtemNrVertices(grafo)];
    int caminho[obtemNrVertices(grafo)][obtemNrVertices(grafo)];
    int nivel[obtemNrVertices(grafo)];
    int visitados[obtemNrVertices(grafo)];
    int fila[obtemNrVertices(grafo)];
    int fim = 0;

    //formatar tudo certinho
    for (int i = 0; i < grafo->numVertices; i++) {
        visitados[i] = 0;
        nivel[i] = -1;
        antecessor[i] = -1;
        for (int j = 0; j < grafo->numVertices; j++) {
            caminho[i][j] = -1;
        }
    }

    //para cada vertice não visitado, visita
    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        if(!visitados[i]) {
            visitados[i] = 1;
            nivel[i] = 0;
            antecessor[i] = -1;
            fila[fim++] = i;

            visitaEmProfundidade(i, visitados, nivel, antecessor, fila, &fim, grafo);
        }
    }

    printf("BP:\n");
    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        printf("%d ", fila[i]);
    }

    //monta os caminhos de cada vertice para o vertice inicial (pai)
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        int v = i;
        while (v != -1) {
            caminho[i][nivel[v]] = v;
            v = antecessor[v];
        }
    }

    printf("\n\n");
    printf("Caminhos BP:\n");
    //imprime caminhos
    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        for(int j = 0; j < obtemNrVertices(grafo); j++) {
            if(caminho[i][j] != -1) {
                printf("%d ", caminho[i][j]);
            }
        }
        printf("\n");
    }
}

void componentesConexos(Grafo *grafo) {
    int visitados[obtemNrVertices(grafo)];
    int antecessor[obtemNrVertices(grafo)];
    int itensComponentes[obtemNrVertices(grafo)];
    int fim = 0, qComponentes = 0;

    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        visitados[i] = 0;
        //fila[i] = -1;
        itensComponentes[i] = -1;
    }

    printf("Componentes conectados:\n");

    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        //se o vertice não foi visitado, visitar
        if(!visitados[i]) {
            //atualizar quantos componentes existem e marca como visitado
            visitados[i] = 1; //cinza
            //fila[fim++] = vertice;
            itensComponentes[fim++] = i;
            qComponentes++;
            visitaEmProfundidadeComponentesConexos(i, visitados, antecessor, itensComponentes, &fim, grafo);

            //ordenar o array de itensComponentes
            ordenarArray(itensComponentes, fim);

            printf("C%d: ", qComponentes);

            //printar os itensComponentes
            int j = 0;
            while(itensComponentes[j] != -1) {
                printf("%d ", itensComponentes[j]);
                //setar os itensComponentes para -1
                //evita um loop a mais
                itensComponentes[j] = -1;
                j++;
            }
            fim = 0;
            printf("\n");
        }
    }
}

void verticesDeArticulacao(Grafo* grafo) {}

int main (int argc, char* argsValues[]) {
    //validar argumentos
    char filename[100] = "";
    strcpy(filename, argsValues[1]);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s.\n", filename);
        return -1;
    }

    int n_vertices, n_arestas;
    fscanf(fp, "%d %d", &n_vertices, &n_arestas);

    Grafo grafo;
    inicializaGrafo(&grafo, n_vertices);

    for(int i = 0; i < n_arestas; i++){
        int v1, v2, peso;
        fscanf(fp, "%d %d %d", &v1, &v2, &peso);
        insereAresta(v1, v2, peso, &grafo);
    }

    /*
    *testando se remove a aresta 1-3
    *testa se retorna o peso da aresta removida
    Peso peso;
    removeArestaObtendoPeso(1, 3, &peso, &grafo);
    printf("Peso da aresta removida: %d\n", peso);

    imprimeGrafo(&grafo);
    */

   /*
   *testando se retorna retorna o primeiro vertice da lista de adj
   
    printf("%d é o primeiro da lista de adj de %d\n", primeiroListaAdj(0, &grafo), 0);
    printf("%d é o primeiro da lista de adj de %d\n", primeiroListaAdj(1, &grafo), 1);
    printf("%d é o primeiro da lista de adj de %d\n", primeiroListaAdj(2, &grafo), 2);
    printf("%d é o primeiro da lista de adj de %d\n", primeiroListaAdj(3, &grafo), 3);
    
    Apontador q = primeiroListaAdj(2, &grafo);
    if(q != NULL) printf("1° adj de %d é o %d\n", 2, q->vdest);
    else printf("Não achamos um vertice\n");
    */

   /*
   *testa se retorna o proximo da lista de adj
    Apontador q = primeiroListaAdj(1, &grafo);
    Apontador p = proxListaAdj(1, &grafo, q);
    if(p != NULL) printf("Na lista %d o promixo de %d é o %d\n", 0, q->vdest, p->vdest);
    */

   /*
    *testa se retorna o valor int do vertice destino
    Apontador q = primeiroListaAdj(2, &grafo);
    printf("Valor int do vertice destino de %d: %d\n", 0, obtemVerticeDestino(q, &grafo));
    */
    
    /*
    *testa libera grafo
    */
    //liberaGrafo(&grafo);
    //if(grafo.numVertices) printf("O grafo ainda existe\n");
    
    buscaEmLargura(&grafo);

    printf("\n");

    buscaEmProfundidade(&grafo);

    printf("\n");
    
    componentesConexos(&grafo);

    printf("\n");

    verticesDeArticulacao(&grafo);

    //imprimeGrafo(&grafo);

    printf("\n");

    printf("FIM DO PROGRAMA!\n");

    return 0;
}
