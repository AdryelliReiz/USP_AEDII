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
bool verificaApontador(Apontador p, Grafo* grafo);

//Funções auxiliares

void visitaEmLargura(int vertice, int visitados[], int nivel[], int antecessor[], int fila[], int *inicio, int *fim, Grafo *grafo) {
    int verticeAdj;
    Apontador apontadorVerticeAdj = primeiroListaAdj(vertice, grafo);
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1; //vertice invalido
    else verticeAdj = obtemVerticeDestino(apontadorVerticeAdj, grafo);

    while(verticeAdj != -1) {
        //coloca o vertices validos na fila
        if(!visitados[verticeAdj]) {
            visitados[verticeAdj] = 1;
            nivel[verticeAdj] = nivel[vertice] + 1;
            antecessor[verticeAdj] = vertice;
            fila[++(*fim)] = verticeAdj;
        }

        //atualiza o vertice a ser verificado
        apontadorVerticeAdj = proxListaAdj(vertice, grafo, apontadorVerticeAdj);
        if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
        else verticeAdj = obtemVerticeDestino(apontadorVerticeAdj, grafo);
    }
    //marca que visitou todos os adjacentes
    visitados[vertice] = 2;

    //se tem elementos na fila, continuar com a visita
    if((*inicio) < (*fim) && (*fim) < obtemNrVertices(grafo)) {
        (*inicio)++;
        visitaEmLargura(fila[*inicio], visitados, nivel, antecessor, fila, inicio, fim, grafo);
    }
}

void visitaEmProfundidade(int vertice, int visitados[], int nivel[], int antecessor[], int fila[], int *fim, Grafo *grafo) {
    int verticeAdj;
    Apontador apontadorVerticeAdj = primeiroListaAdj(vertice, grafo);
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
    else verticeAdj = obtemVerticeDestino(apontadorVerticeAdj, grafo);

    //enquanto o vértice é válido, se não foi visitado-> visita, senão procura outro adjacente
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
        else verticeAdj = obtemVerticeDestino(apontadorVerticeAdj, grafo);
    }
}

void visitaEmProfundidadeComponentesConexos(int vertice, int visitados[], int antecessor[], int fila[], int *fim, Grafo *grafo) {
    int verticeAdj;
    Apontador apontadorVerticeAdj = primeiroListaAdj(vertice, grafo);
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
    else verticeAdj = obtemVerticeDestino(apontadorVerticeAdj, grafo);
    
    //enquanto o vértice é válido, se não foi visitado-> visita, senão procura outro adjacente
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
        else verticeAdj = obtemVerticeDestino(apontadorVerticeAdj, grafo);
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

//função DFS para identificar vértices de articulação
void dsfArticulacao(int u, int pai, int *visit, int *tempo_desc, int *menor_desc, bool *articulacao, int *time_s, Grafo *grafo) {
    visit[u] = 1;
    tempo_desc[u] = menor_desc[u] = ++(*time_s);
    int filhos = 0; //contador de filhos na árvore de busca em profundidade

    //enquanto o vértice é válido, se não foi visitado-> visita, senão procura outro adjacente
    for (Apontador v = primeiroListaAdj(u, grafo); verificaApontador(v, grafo); v = proxListaAdj(u, grafo, v)) {
        int verticeAdj = obtemVerticeDestino(v, grafo);

        //se o vertice não foi visitado, visitar
        if (!visit[verticeAdj]) {
            filhos++;
            dsfArticulacao(verticeAdj, u, visit, tempo_desc, menor_desc, articulacao, time_s, grafo); // Chama recursivamente a DFS para v

            //atualiza o menor tempo
            menor_desc[u] = menor_desc[u] < menor_desc[verticeAdj] ? menor_desc[u] : menor_desc[verticeAdj];

            //verifica se u é um vértice de articulação
            //verifica se o vertice é um vertice de articulação
            //1. u é raiz da árvore de busca em profundidade e tem mais de um filho
            //2. u não é raiz da árvore de busca em profundidade e menor_desc[verticeAdj] >= tempo_desc[u]
            if ((pai == -1 && filhos > 1) || (pai != -1 && menor_desc[verticeAdj] >= tempo_desc[u])) {
                articulacao[u] = true;
            }
        } //se v já foi visitado e não é o pai de u
        else if (verticeAdj != pai) {
            menor_desc[u] = menor_desc[u] < tempo_desc[verticeAdj] ? menor_desc[u] : tempo_desc[verticeAdj];
        }
    }
}

//Funções principais que usam as funções acima '0'

//função que realiza a busca em largura em um grafo
void buscaEmLargura(Grafo *grafo, FILE *arquivoSaida) {
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
            fila[fim] = i;
            
            visitaEmLargura(i, visitados, nivel, antecessor, fila, &inicio, &fim, grafo);

            visitados[i] = 2; //marca como preto pq acabou os vertices adjacentes
        }
    }

    fprintf(arquivoSaida, "BL:\n");
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        fprintf(arquivoSaida, "%d ", fila[i]);
    }
    
    //monta os caminhos de cada vertice para o vertice inicial (antecessor)
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        int v = i;
        while (v != -1) {
            caminho[i][nivel[v]] = v;
            v = antecessor[v];
        }
    }

    fprintf(arquivoSaida, "\n\n");
    fprintf(arquivoSaida, "Caminhos BL:\n");
    //imprime caminhos
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        for (int j = 0; j < obtemNrVertices(grafo); j++) {
            if (caminho[i][j] != -1) {
                fprintf(arquivoSaida, "%d ", caminho[i][j]);
            }
        }
        fprintf(arquivoSaida, "\n");
    }
    fprintf(arquivoSaida, "\n");
}

//função que realiza a busca em profundidade de um grafo
void buscaEmProfundidade(Grafo *grafo, FILE *arquivoSaida) {
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

    fprintf(arquivoSaida, "BP:\n");
    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        fprintf(arquivoSaida, "%d ", fila[i]);
    }

    //monta os caminhos de cada vertice para o vertice inicial (pai)
    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        int v = i;
        while (v != -1) {
            caminho[i][nivel[v]] = v;
            v = antecessor[v];
        }
    }

    fprintf(arquivoSaida, "\n\n");
    fprintf(arquivoSaida, "Caminhos BP:\n");
    //imprime caminhos
    for(int i = 0; i < obtemNrVertices(grafo); i++) {
        for(int j = 0; j < obtemNrVertices(grafo); j++) {
            if(caminho[i][j] != -1) {
                fprintf(arquivoSaida, "%d ", caminho[i][j]);
            }
        }
        fprintf(arquivoSaida, "\n");
    }
    fprintf(arquivoSaida, "\n");
}

//função para identificar componentes conexos
void componentesConexos(Grafo *grafo, FILE *arquivoSaida) {
    int visitados[obtemNrVertices(grafo)];
    int antecessor[obtemNrVertices(grafo)];
    int itensComponentes[obtemNrVertices(grafo)];
    int fim = 0, qComponentes = 0;

    for (int i = 0; i < obtemNrVertices(grafo); i++) {
        visitados[i] = 0;
        //fila[i] = -1;
        itensComponentes[i] = -1;
    }

    fprintf(arquivoSaida, "Componentes Conectados:\n");

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

            fprintf(arquivoSaida, "C%d: ", qComponentes);

            //printar os itensComponentes
            int j = 0;
            while(j < fim && itensComponentes[j] != -1) {
                fprintf(arquivoSaida, "%d ", itensComponentes[j]);
                //setar os itensComponentes para -1
                //evita um loop a mais
                itensComponentes[j] = -1;
                j++;
            }
            fim = 0;
            fprintf(arquivoSaida, "\n");
        }
    }
    fprintf(arquivoSaida, "\n");
}

//função para identificar e imprimir os vértices de articulação
void verticesDeArticulacao(Grafo* grafo, FILE *arquivoSaida) {
    int visit[grafo->numVertices];
    int tempo_desc[grafo->numVertices];
    int menor_desc[grafo->numVertices];
    bool articulacao[grafo->numVertices];
    int time_s = 0;

    //inicializa os arrays
    for (int i = 0; i < grafo->numVertices; i++) {
        visit[i] = 0;
        tempo_desc[i] = 0;
        menor_desc[i] = 0;
        articulacao[i] = false;
    }

    //executa a DFS para cada vértice não visitado
    for (int i = 0; i < grafo->numVertices; i++) {
        if (!visit[i]) {
            dsfArticulacao(i, -1, visit, tempo_desc, menor_desc, articulacao, &time_s, grafo);
        }
    }

    fprintf(arquivoSaida, "Vertices de articulacao:\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        if (articulacao[i]) {
            fprintf(arquivoSaida, "%d ", i);
        }
    }
    fprintf(arquivoSaida, "\n");
}

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

    //criando arquivo de saída
    char exitFilename[100] = "";
    strcpy(exitFilename, argsValues[2]);
    FILE *arquivoSaida = fopen(exitFilename, "w");

    buscaEmLargura(&grafo, arquivoSaida);

    buscaEmProfundidade(&grafo, arquivoSaida);

    componentesConexos(&grafo, arquivoSaida);

    verticesDeArticulacao(&grafo, arquivoSaida);
    
    liberaGrafo(&grafo);

    return 0;
}
