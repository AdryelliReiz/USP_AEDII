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
    if(!verificaApontador(apontadorVerticeAdj, grafo)) verticeAdj = -1;
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
    if(inicio != fim && (*fim) < obtemNrVertices(grafo)) {
        (*inicio)++;
        visitaEmLargura(fila[*inicio], visitados, nivel, antecessor, fila, inicio, fim, grafo);
    }
}

void visitaEmProfundidade(int vertice, int visitados[], int nivel[], int antecessor[], int fila[], int *inicio, int *fim, Grafo *grafo) {
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
            visitaEmProfundidade(verticeAdj, visitados, nivel, antecessor, fila, inicio, fim, grafo);
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
void buscaEmLargura(Grafo *grafo, int vertice) {
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

    visitados[vertice] = 1;
    nivel[vertice] = 0;
    antecessor[vertice] = -1;
    fila[fim++] = vertice;

    visitaEmLargura(vertice, visitados, nivel, antecessor, fila, &inicio, &fim, grafo);

    //verifica se chegou ao fim da busca e se ainda tem vertices para visitar
    while(inicio == fim && (fim + 1) < obtemNrVertices(grafo)) {
        //acha o vertice
        for(int i = 0; i < obtemNrVertices(grafo); i++) {
            if(!visitados[i]) {
                visitados[i] = 1;
                nivel[i] = 0;
                antecessor[i] = -1;
                fila[fim++] = i;
                break;
            }
        }
        //se achou, começa uma nova busca
        if(inicio != fim) visitaEmLargura(fila[inicio++], visitados, nivel, antecessor, fila, &inicio, &fim, grafo);
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

void buscaEmProfundidade(Grafo *grafo, int vertice) {
    int antecessor[obtemNrVertices(grafo)];
    int caminho[obtemNrVertices(grafo)][obtemNrVertices(grafo)];
    int nivel[obtemNrVertices(grafo)];
    int visitados[obtemNrVertices(grafo)];
    int fila[obtemNrVertices(grafo)];
    int inicio = 0, fim = 0;

    //formatar tudo certinho
    for (int i = 0; i < grafo->numVertices; i++) {
        visitados[i] = 0;
        nivel[i] = -1;
        antecessor[i] = -1;
        for (int j = 0; j < grafo->numVertices; j++) {
            caminho[i][j] = -1;
        }
    }

    //começa a visita
    visitados[vertice] = 1;
    nivel[vertice] = 0;
    antecessor[vertice] = -1;
    fila[fim++] = vertice;

    visitaEmProfundidade(vertice, visitados, nivel, antecessor, fila, &inicio, &fim, grafo);
    //verifica se chegou ao fim da busca e se ainda tem vertices para visitar
    while(inicio == fim && (fim + 1) < obtemNrVertices(grafo)) {
        //acha o vertice
        for(int i = 0; i < obtemNrVertices(grafo); i++) {
            if(!visitados[i]) {
                visitados[i] = 1;
                nivel[i] = 0;
                antecessor[i] = -1;
                fila[fim++] = i;
                break;
            }
        }
        //se achou, começa uma nova busca
        if(inicio != fim) visitaEmProfundidade(fila[inicio++], visitados, nivel, antecessor, fila, &inicio, &fim, grafo);
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

void componentesConexos(Grafo *grafo, int vertice) {
    int visitados[obtemNrVertices(grafo)];
    int fila[obtemNrVertices(grafo)];
    int itensComponentes[obtemNrVertices(grafo)];
    int inicio = 0, fim = 0, qComponentes = 0, indexComponente = 0;

    for (int i = 0; i < grafo->numVertices; i++) {
        visitados[i] = 0;
        fila[i] = -1;
        itensComponentes[i] = -1;
    }

    printf("Componentes conectados:\n");

    //adicionar o primeiro componente
    visitados[vertice] = 1; //cinza
    fila[fim++] = vertice;
    itensComponentes[indexComponente] = vertice;
    qComponentes++;

    while(inicio != fim) {
        vertice = fila[inicio++];
        int vAdj;
        Apontador apontadorVAdj = primeiroListaAdj(vertice, grafo);
        //vertice valido
        if(!verificaApontador(apontadorVAdj, grafo)) vAdj = -1;
        else vAdj = obtemValorVertice(apontadorVAdj, grafo);
        
        while(vAdj != -1) {
            if(!visitados[vAdj]) {
                visitados[vAdj] = 1;
                fila[fim++] = vAdj;
                itensComponentes[++indexComponente] = vAdj;
            }
            apontadorVAdj = proxListaAdj(vertice, grafo, apontadorVAdj);
            if(!verificaApontador(apontadorVAdj, grafo)) vAdj = -1;
            else vAdj = obtemValorVertice(apontadorVAdj, grafo);
        }

        //visitou todos os adjacentes
        visitados[vertice] = 2;

        //procurar outro componente
        if(inicio == fim) {
            //função de ordenação antes de imprimir os resultados!!
            ordenarArray(itensComponentes, indexComponente + 1);
            printf("C%d: ", qComponentes);
            for(int i = 0; i <= indexComponente; i++) {
                printf("%d ", itensComponentes[i]);
            }
            printf("\n");

            for(int i  = 0; i < obtemNrVertices(grafo); i++) {
                if(!visitados[i]) {
                    //setar a lista de componentes
                    for(int j = 0; j < obtemNrVertices(grafo); j++) {
                        itensComponentes[j] = -1;
                    }
                    indexComponente = 0;
                    visitados[i] = 1;
                    fila[fim++] = i;
                    itensComponentes[indexComponente] = i;
                    qComponentes++;
                    break;
                }
            }
        }
    }

}

void verticesDeArticulacao(Grafo *grafo);

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
    
    buscaEmLargura(&grafo, 0);

    printf("\n");

    buscaEmProfundidade(&grafo, 0);

    printf("\n");
    
    componentesConexos(&grafo, 0);

    //imprimeGrafo(&grafo);

    printf("\n");

    printf("FIM DO PROGRAMA!\n");

    return 0;
}
