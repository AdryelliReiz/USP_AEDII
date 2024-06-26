/***********************************/
/* Nome: Adryelli Reis dos Santos  */
/* Número USP: 14714019            */
/***********************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define GRAU_T 3

//estruturas
//***************************************************************************************************
typedef int KeyType;
typedef struct str_no {
	KeyType key[2*GRAU_T-1];
	struct str_no* children[2*GRAU_T];
	int numKeys;
	bool leaf;
} NO;
typedef struct {
	NO* root;
} B_tree_plus;


//funções auxiliares
//***************************************************************************************************

//Função para criar um novo nó
NO* allocateNode(bool leaf) {
    NO* node = (NO*)malloc(sizeof(NO));
    node->leaf = leaf;
    node->numKeys = 0;

    for (int i = 0; i < 2*GRAU_T; i++) {
        node->children[i] = NULL;
    }

    return node;
}

NO* findNextLeaf(NO* root, KeyType key, KeyType* nextKey) {
    NO* node = root;

    while (!node->leaf) {
        int i = 0;
        while (i < node->numKeys && key >= node->key[i]) {
            i++;
        }
        //atualiza a próxima chave do próximo nó folha
        if (i < node->numKeys) {
            *nextKey = node->key[i];
        }
        node = node->children[i];
    }

    if (node->numKeys > 0 && key < node->key[0]) {
        *nextKey = node->key[0];
    }

    return node;
}


//função para conectar os nós folha
void connectLeaves(NO* root) {
    if (root == NULL) return;

    NO* previousLeaf = NULL;
    NO* currentLeaf = NULL;
    int previousKey = -1;

    //encontra o nó folha mais à esquerda
    NO* node = root;
    while (!node->leaf) {
        previousKey = node->key[0];
        node = node->children[0];
    }
    currentLeaf = node;

    while (currentLeaf != NULL) {
        //se o nó folha anterior existir, conecta-o ao nó folha atual
        if (previousLeaf != NULL) {
            previousLeaf->children[2*GRAU_T-1] = currentLeaf;
        }
        previousLeaf = currentLeaf;

        //move para o próximo nó folha e atualiza a chave anterior
        if (
            previousKey != currentLeaf->key[0] && //a chave anterior é diferente da primeira chave do nó folha atual
            previousKey != -1 //não é a raiz
        ) {
            currentLeaf = findNextLeaf(root, previousKey, &previousKey);
        } else {
            currentLeaf = NULL;
        }
    }
}

NO* B_Tree_Plus_Bin_Search(NO* node, KeyType key, NO** antNode, int* indexPointer) {
    NO* no_finded = NULL;
    *indexPointer = 0; //reseta 

    while(node != NULL) {
        int indexKey = 0;

        //procurar a chave no nó
        while(indexKey < node->numKeys && key > node->key[indexKey]) {
            indexKey++;
        }

        //verificar se a chave foi encontrada
        if(indexKey < node->numKeys && key == node->key[indexKey]) {
            no_finded = node;

            if(node->leaf) {
                break;
            } else {
                *indexPointer = indexKey + 1;
                *antNode = node;
                //continuar a busca no nó filho a direita (pq ele é maior ou igual ao nó que já achamos)
                node = node->children[indexKey + 1];
            }
        } else {
            *indexPointer = indexKey;
            *antNode = node;
            //continuar a busca no nó filho
            node = node->children[indexKey];
        }

        //verificar se o nó é folha
        if(no_finded && no_finded->leaf) {
            break;
        }
    }    

    return no_finded;
}

//funçẽs principais
//***************************************************************************************************

//função de impressão da árvore B+
void Print_B_Tree_Plus(NO* no, FILE* efp){
    if(no->numKeys == 0) {
        fprintf(efp, "Vazia");
        return;
    };

    //verifica se o nó é folha
    if(no->leaf){
        fprintf(efp, "(");
        //percorre as chaves do nó folha
        for(int i = 0; i < no->numKeys; i++){
            fprintf(efp, "%d", no->key[i]);
            if(i < no->numKeys - 1){
                fprintf(efp, " ");
            }
        }
        fprintf(efp, ")");
    } else {
        //nó não é folha
        fprintf(efp, "(");
        //percorre as chaves do nó até chegar na folha
        for(int i = 0; i <= no->numKeys; i++){
            //chama a função de print para o próximo nó filho
            Print_B_Tree_Plus(no->children[i], efp);
            if(i < no->numKeys) {
                fprintf(efp, " %d ", no->key[i]);
            }
        }
        fprintf(efp, ")");
    }
}

//função para dividir o filho y do nó x
void B_Tree_Split_Child(NO* x, int i, NO* y) {
    NO* newNode = allocateNode(y->leaf);//cria um novo nó

    //copia as chaves de y para o novo nó
    for (int j = 0; j < GRAU_T -1; j++) {
        newNode->key[j] = y->key[j+GRAU_T];
        //deleta as chaves antigas
        y->key[j+GRAU_T] = -1;
        //atualiza o número de chaves
        newNode->numKeys++;
        y->numKeys--;
    }

    //copia os filhos de y para o novo nó
    if (!y->leaf) {
        for (int j = 0; j < GRAU_T; j++) {
            newNode->children[j] = y->children[j+GRAU_T];
            //deleta os filhos antigos
            y->children[j+GRAU_T] = NULL;
        }
    }

    //move os filhos de x para abrir espaço para o novo filho
    for (int j = x->numKeys; j > i; j--) { //!!! Altertei mas n sei se tá totalmente
        x->children[j+1] = x->children[j]; //ok
    }

    //liga o novo nó a x
    x->children[i+1] = newNode; //OK

    //move as chaves de x para abrir espaço para a nova chave
    for (int j = x->numKeys - 1; j >= i; j--) {
        x->key[j+1] = x->key[j];
    }

    //copia a chave mediana de y para x
    x->key[i] = y->key[GRAU_T-1];
    y->key[GRAU_T-1] = -1;
    x->numKeys++;
    y->numKeys--;

    //adiciona a chave que subiu para x na folha
    //CASO SE Y FOR FOLHA
    if (newNode->leaf) {
        //move as chaves de newNode para dar expaço para a nova chave
        for (int j = newNode->numKeys - 1; j >= 0; j--) {
            newNode->key[j+1] = newNode->key[j];
        }
        newNode->key[0] = x->key[i];
        newNode->numKeys++;
    }
}

//função para inserir a chave k em um nó não cheio x
void B_Tree_Insert_NonFull(NO* x, KeyType k) {
    int i = x->numKeys - 1;

    if (x->leaf) {
        //move as chaves maiores para abrir espaço para a nova chave
        while (i >= 0 && k < x->key[i]) {
            x->key[i+1] = x->key[i];
            i--;
        }
        //insere a nova chave
        x->key[i+1] = k;
        x->numKeys += 1;
    } else {
        while (i >= 0 && k < x->key[i]) {
            i--;
        }
        i++;
        if (x->children[i]->numKeys == 2*GRAU_T-1) {
            B_Tree_Split_Child(x, i, x->children[i]);
            if (k > x->key[i]) {
                i++;
            }
        }
        B_Tree_Insert_NonFull(x->children[i], k);
    }
}

//função para inserir a chave k na árvore T
void B_Tree_Insert(B_tree_plus* T, KeyType k) {
    NO* r = T->root;
    //caso a raiz esteja cheia
    if (r->numKeys == 2*GRAU_T-1) {
        NO* s = allocateNode(false);//cria um novo nó
        T->root = s;//atualiza a raiz
        s->children[0] = r;//faz a raiz antiga ser a filha do novo nó
        B_Tree_Split_Child(s, 0, r);//divide o filho da raiz
        B_Tree_Insert_NonFull(s, k);//insere a chave k a partir do novo nó
    } else {
        B_Tree_Insert_NonFull(r, k);//insere a chave k a partir da raiz
    }
    connectLeaves(T->root);
}

//função de remoção de uma chave (se ela existir)
bool removeNode(NO* root, NO* node_key, KeyType key, NO* antNode, int indexPointer) {
    if(node_key == NULL) {
        return false;
    }

    int i = 0;
    //procura a chave no nó
    while(i < node_key->numKeys && key > node_key->key[i]) {
        i++;
    }

    //verifica se o nó é folha
    if(node_key->leaf) {
        //se o nó folha for a raiz
        if(antNode == NULL) {
            //atualiza o nó removendo a chave
            for(int j = i; j < node_key->numKeys - 1; j++) {
                node_key->key[j] = node_key->key[j+1];
                //limpa
                node_key->key[j+1] = -1;
            }
            node_key->numKeys--;
        } else {
            //nó não é raiz
            if(node_key->numKeys > GRAU_T - 1) {
                //caso 1: o nó folha tem pelo menos GRAU_T chaves
                //atualiza o nó removendo a chave
                for(int j = i; j < node_key->numKeys - 1; j++) {
                    node_key->key[j] = node_key->key[j+1];
                    //limpa
                    node_key->key[j+1] = -1;
                }
                //atualiza o número de chaves
                node_key->numKeys--;
            } else {
                //caso 2: o nó folha tem GRAU_T - 1 chaves
                //verifica se o irmão a esquerda tem pelo menos GRAU_T chaves
                if(indexPointer > 0 && antNode->children[indexPointer-1]->numKeys >= GRAU_T) {
                    //encontra o irmão a esquerda
                    NO* leftSibling = antNode->children[indexPointer-1];

                    //remove a chave e atualiza o nó
                    for(int j = i; j < node_key->numKeys - 1; j++) {
                        node_key->key[j] = node_key->key[j+1];
                        //limpa
                        node_key->key[j+1] = -1;
                    }
                    //atualiza o número de chaves
                    node_key->numKeys--;

                    //veriica se a chave que vai descer é igual a primeira chave do filho
                    if(antNode->key[indexPointer -1] != node_key->key[0]) {
                        //abre espaço para a nova chave pai no nó folha
                        for(int j = node_key->numKeys; j > 0; j--) {
                            node_key->key[j] = node_key->key[j-1];
                        }
                        //atualiza a chave copia do pai na folha
                        node_key->key[0] = antNode->key[indexPointer - 1];
                        node_key->numKeys++;
                    }

                    //move a maior chave do irmão a esquerda para o nó pai
                    antNode->key[indexPointer - 1] = leftSibling->key[leftSibling->numKeys - 1];
                    
                    //abre espaço para a copia da chave do novo pai
                    for(int j = node_key->numKeys; j > 0; j--) {
                        node_key->key[j] = node_key->key[j-1];
                    }
                    //atualiza a chave copia do pai no nó folha
                    node_key->key[0] = leftSibling->key[leftSibling->numKeys - 1];
                    node_key->numKeys++;

                    //atualiza o número de chaves do nó esquerda
                    leftSibling->key[leftSibling->numKeys - 1] = -1;
                    leftSibling->numKeys--;
                } else if(indexPointer < antNode->numKeys && antNode->children[indexPointer+1]->numKeys >= GRAU_T) {
                    //caso 3: o irmão a direita tem pelo menos GRAU_T chaves
                    //encontra o irmão a direita
                    NO* rightSibling = antNode->children[indexPointer+1];

                    //remove a chave e atualiza o nó
                    for(int j = i; j < node_key->numKeys - 1; j++) {
                        node_key->key[j] = node_key->key[j+1];
                        //limpa
                        node_key->key[j+1] = -1;
                    }

                    //move a chave pai para o nó folha
                    node_key->key[node_key->numKeys - 1] = antNode->key[indexPointer];

                    //atualiza o irmão a direita
                    for(int j = 0; j < rightSibling->numKeys - 1; j++) {
                        rightSibling->key[j] = rightSibling->key[j+1];
                        //limpa
                        rightSibling->key[j+1] = -1;
                    }
                    //atualiza o número de chaves do irmão a direita
                    rightSibling->numKeys--;

                    //atualiza a chave copia do pai no nó folha
                    antNode->key[indexPointer] = rightSibling->key[0];
                } else {
                    //caso 4: une o nó folha com o irmão
                    //precisamos saber qual irmão ele tem

                    //remove a chave e atualiza o nó
                    for(int j = i; j < node_key->numKeys - 1; j++) {
                        node_key->key[j] = node_key->key[j+1];
                        //limpa
                        node_key->key[j+1] = -1;
                    }
                    node_key->numKeys--;

                    //TODO: fazer o caso em que o nó pai (não raiz) tem T-1 chaves e seu nó não pode ter menos
                    //precisa descer a chave do pai para o nó atual

                    if(indexPointer > 0) {
                        //o irmão é o da esquerda
                        NO* leftSibling = antNode->children[indexPointer-1];

                        //copiar a chave pai para o nó folha (que esta a esquerda)
                        leftSibling->key[leftSibling->numKeys] = antNode->key[indexPointer-1];
                        leftSibling->numKeys++;

                        //atualiza as chaves do pai
                        for(int j = indexPointer -1; j < antNode->numKeys - 1; j++) {
                            antNode->key[j] = antNode->key[j+1];
                        }
                        
                        antNode->key[antNode->numKeys - 1] = -1;

                        //atualiza ponteiros do pai
                        for(int j = indexPointer; j < antNode->numKeys; j++) {
                            antNode->children[j] = antNode->children[j+1];
                        }
                        antNode->children[antNode->numKeys] = NULL;
                        antNode->numKeys--;

                        int count = 0;
                        //verifica se a primeira chave a ser tranferida para o irmão é o igual ao ultimo elemento do irmão
                        if(node_key->key[0] == leftSibling->key[leftSibling->numKeys - 1]) {
                            count++; //começa a copiar a partir da segunda chave
                        }

                        //copia as chaves do nó folha para o irmão
                        for(int j = count; j < node_key->numKeys; j++) {//começa no 1 pq  a chave 0 é a chave que está no pai
                            leftSibling->key[leftSibling->numKeys] = node_key->key[j];
                            leftSibling->numKeys++;
                        }

                        //desaloca o nó folha
                        free(node_key);
                    } else {
                        //o irmão é o da direita
                        NO* rightSibling = antNode->children[indexPointer+1];

                        //atualiza as chaves do pai
                        for(int j = indexPointer; j < antNode->numKeys; j++) {
                            antNode->key[j] = antNode->key[j+1];
                        }
                        
                        antNode->key[antNode->numKeys - 1] = -1;
                        
                        //atualiza os ponteiros do pai
                        for(int j = indexPointer + 1; j < antNode->numKeys; j++) {
                            antNode->children[j] = antNode->children[j+1];
                        }
                        antNode->children[antNode->numKeys] = NULL;
                        antNode->numKeys--;

                        //copia as chaves do nó irmão para o folha
                        for(int j = 0; j < rightSibling->numKeys; j++) {
                            node_key->key[node_key->numKeys] = rightSibling->key[j];
                            node_key->numKeys++;
                        }

                        //desaloca o nó irmão
                        free(rightSibling);
                    }
                }
            }
        }
    } else {
        //se não for folha

        //verifica se a chave mais a direita tem pelo menos GRAU_T chaves
        //ou se a cahve mais a esquerda tem pelo menos GRAU_T chaves
        //encontra a chave predecessor
        NO* antPreced = node_key;
        NO* predecessor = node_key->children[i];
        while(!predecessor->leaf) {
            antPreced = predecessor;
            predecessor = predecessor->children[predecessor->numKeys];
        }

        if(predecessor->numKeys >= GRAU_T) {
            //caso 1: o seu filho a esquerda tem pelo menos GRAU_T chaves

            //encontra a chave predecessor
            KeyType pred = predecessor->key[predecessor->numKeys - 1];
            //remove a chave predecessor
            predecessor->key[predecessor->numKeys - 1] = -1;
            //atualiza a chave predecessor
            predecessor->numKeys--;

            //substitui a chave removida pela chave predecessor
            node_key->key[i] = pred;
            removeNode(root, predecessor, pred, antPreced, antPreced->numKeys - 1);
        } else {
            //não dá para remover a chave predecessor, tentamos remover o sucessor
            
            //encontra a chave sucessor
            NO* sucessor = node_key->children[i+1];
            while(!sucessor->leaf) {
                sucessor = sucessor->children[0];
            }

            //caso 2 (SUCESSOR): o seu filho a direita tem pelo menos GRAU_T chaves
            //encontra a chave sucessor
            KeyType suc = sucessor->key[0];

            //substitui a chave removida pela chave sucessor
            node_key->key[i] = suc;
        }
    }

    return true;
}

//função de remoção de uma chave k da árvore B+ pela raiz
void B_Tree_Delete_From_Root(B_tree_plus* T, KeyType k){
    NO* root = T->root;
    //verifica se a árvore está vazia
    if(root->numKeys == 0) return;
    else {
        int indexPointer = 0;
        NO* antNode = NULL;
        NO* node_key = B_Tree_Plus_Bin_Search(root, k, &antNode, &indexPointer);

        //enquanto a chave ainda existir na árvore, remover
        while(node_key != NULL) {
            removeNode(root, node_key, k, antNode, indexPointer);
            //procura se ainda existe a chave
            node_key = B_Tree_Plus_Bin_Search(root, k, &antNode, &indexPointer);
        }
    }

    //verifica se a raiz ficou vazia
    if(root->numKeys == 0){
        NO* temp = root;
        root = root->children[0];
        free(temp);
    }

    connectLeaves(T->root);

    return;
}

//***************************************************************************************************

int main(int argc, char* argsValues[]) {
    //validar argumentos
    if (argc < 2) {
        fprintf(stderr, "Falta argumentos\n");
        return 1;
    }

    //abrindo arquivo
    char filename[100] = "";
    strcpy(filename, argsValues[1]);
    FILE* fp = fopen(filename, "r");

    if (fp == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s.\n", filename);
        return -1;
    }

    //criando arquiv de saida
    char exitFilename[100] = "";
    strcpy(exitFilename, argsValues[2]);
    FILE *efp = fopen(exitFilename, "w");

    //criando árvore B+
    B_tree_plus T;
    T.root = allocateNode(true);

    //lendo comandos
    char command = ' ';
    int key;
    while (fscanf(fp, "%c", &command) && command != ' ' && command != 'f'){
        switch (command) {
        //comando de inserção
        case 'i':
            fscanf(fp, "%d", &key);
            B_Tree_Insert(&T, key);

            break;

        //comando de remoção
        case 'r':
            fscanf(fp, "%d", &key);
            B_Tree_Delete_From_Root(&T, key);

            break;

        //comando de impressão
        case 'p':
            Print_B_Tree_Plus(T.root, efp);
            fprintf(efp,"\n");
            break;

        default:
            break;
        }

        //reseta o comando
        command = ' ';
    }

    //fechando arquivos
    fclose(fp);
    fclose(efp);

    //desalocando a árvore
    free(T.root);
    
    return 0;
}