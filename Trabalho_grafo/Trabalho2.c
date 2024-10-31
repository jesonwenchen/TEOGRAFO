#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>

typedef struct Node {// Estrutura de nó para a lista adj
    int vertex;
    float peso;
    struct Node* next;
} Node;

typedef struct { // estrutura de no para o heap
    int vertex;
    float key; // peso
} MinHeapNode;

typedef struct { // MinHeap
    int capacity;
    int size; 
    MinHeapNode* array;
    int* pos; // ponteiro que apontam para a posição do nó.
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->capacity = capacity;
    minHeap->size = 0;
    minHeap->array = (MinHeapNode*)malloc(capacity * sizeof(MinHeapNode));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode* a, MinHeapNode* b) {
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left].key < minHeap->array[smallest].key)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right].key < minHeap->array[smallest].key)
        smallest = right;

    if (smallest != idx) {
        // Atualize o mapa de posições antes de trocar
        minHeap->pos[minHeap->array[smallest].vertex] = idx;
        minHeap->pos[minHeap->array[idx].vertex] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

MinHeapNode extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0)
        return (MinHeapNode){-1, FLT_MAX};

    MinHeapNode root = minHeap->array[0];

    if (minHeap->size > 1) {
        minHeap->array[0] = minHeap->array[minHeap->size - 1];
        
        // Atualize o mapa de posições
        minHeap->pos[minHeap->array[0].vertex] = 0;
        
        minHeapify(minHeap, 0);
    }

    minHeap->size--;
    return root;
}

void decreaseKey(MinHeap* minHeap, int vertex, float key) {
    int i = minHeap->pos[vertex];
    minHeap->array[i].key = key;

    // Ajusta a posição para cima no heap
    while (i && minHeap->array[i].key < minHeap->array[(i - 1) / 2].key) {
        // Atualize o mapa de posições antes de trocar
        minHeap->pos[minHeap->array[i].vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2].vertex] = i;

        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

float** matrizpeso(FILE*grafo, int* n, FILE* qvertice){
    if (grafo == NULL){
        printf("Erro ao abrir\n");
        return NULL;
    }
    if (fscanf(grafo, "%d", n) != 1){
        printf("Erro ao ler o número de vértices\n");
        return NULL;
    }
    float** matriz = (float**)malloc(*n * sizeof(float*));
    for (int i = 0; i < *n; i++){
        matriz[i] = (float*)malloc(*n*sizeof(float));
        for (int j = 0; j < *n; j++){
            if (i == j){
                matriz[i][j] = 0;
            } else {
                matriz[i][j] = FLT_MAX;
            }
        }
    }
    int i, j; 
    float peso;
    
    while(fscanf(grafo, "%d %d %f", &i, &j, &peso) == 3){
        if (peso < 0) {
            fprintf(qvertice,"A biblioteca ainda não implementa caminhos mínimos com pesos negativos \n");
            return NULL;
        }
        else {  
            matriz[i - 1][j - 1] = peso;
            matriz[j - 1][i - 1] = peso;
        }
    }
    return matriz;
}

void criandolista(Node** adjList, int a, int b, float peso) {// Função para adicionar um vértice à lista de adjacência
    Node* no = (Node*)malloc(sizeof(Node));
    no->vertex = b;
    no->peso = peso;
    Node* current = adjList[a];
    while (current != NULL) {
        if (current->vertex == b) {
            // Se o vértice já existe na lista, sai da função
            return;
        }
        current = current->next;
    }
    no->next = adjList[a];
    adjList[a] = no;
}

Node** lista(FILE*grafo, int* n, FILE* qvertice){
    if (grafo == NULL){
        printf("Erro ao abrir\n");
    }
    if (fscanf(grafo, "%d", n) != 1){
        printf("Erro ao ler o número de vértices\n");
        return NULL;
    }    
    Node** lista = (Node**)malloc(*n * sizeof(Node*));
    for (int i = 0; i< *n ; i++){
        lista[i] = NULL;
    }
    int origem, destino;
    float peso;
    while (fscanf(grafo, "%d %d %f", &origem, &destino, &peso) == 3) {
        if (peso < 0){ 
            fprintf(qvertice, "A biblioteca ainda não implementa caminhos mínimos com pesos negativos \n");
            return NULL; 
        } 
        else {
        criandolista(lista, origem - 1, destino - 1, peso);
        criandolista(lista, destino - 1, origem - 1, peso); 
        }
    }
    return lista;
}

void caminhominimo(int* pai, int vAtual, FILE* qvertice) {
    if (pai[vAtual] == -1){
        fprintf(qvertice,"%d",vAtual + 1);
        return; // para nao continuar mais
    }
    caminhominimo(pai, pai[vAtual], qvertice);
    fprintf(qvertice, " -> %d", vAtual + 1);
    
}

void Dijkstra_Matriz(float**matriz, int vertx, int n, FILE*qvertice){
    float* dist = (float*)malloc(n * sizeof(float));
    int* VS = (int*)malloc(n * sizeof(int));
    int* pai = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        pai[i] = -1;
        dist[i] = FLT_MAX;
        VS[i] = 0;
    } 
    dist[vertx - 1] = 0;

    for (int i = 0; i < n - 1; i++){
        int u = -1;
        float min = FLT_MAX;
        for (int v = 0; v < n; v++){            
            if (VS[v] == 0 && dist[v] < min){
                min = dist[v];
                u = v;
            }
        }
        VS[u] = 1;
        for (int v = 0; v < n; v++){
            if (!VS[v] && matriz[u][v] != FLT_MAX && dist[u] != FLT_MAX && dist[u] + matriz[u][v] < dist[v]){
                dist[v] = dist[u] + matriz[u][v];
                pai[v] = u;
            }
        }
    }

    fprintf(qvertice, "Distâncias mínimas a partir do vértice %d:\n", vertx );
    for (int i = 0; i < n; i++) {
        fprintf(qvertice, "Vértice %d, Distância: %.2f, Caminho: ", i + 1, dist[i]);
        caminhominimo(pai, i , qvertice);
        fprintf(qvertice, "\n");
    }
}

void Dijkstra_Lista(Node* lista[], int vertx, int n, FILE*qvertice){
    float* dist = (float*)malloc(n * sizeof(float));
    int* VS = (int*)malloc(n * sizeof(int));
    int* pai = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        pai[i] = -1;
        dist[i] = FLT_MAX;
        VS[i] = 0;
    } 
    dist[vertx - 1] = 0;

    for (int i = 0; i < n - 1; i++){
        int u = -1;
        float min = FLT_MAX;
        for (int v = 0; v < n; v++){            
            if (VS[v] == 0 && dist[v] < min){
                min = dist[v];
                u = v;
            }
        }

        VS[u] = 1;

        Node* ponteiro = lista[u];
        while(ponteiro!=NULL){
            int v = ponteiro -> vertex; 
            float peso = ponteiro -> peso;
            if (!VS[v] && dist[u] + peso < dist[v]){
                dist[v] = dist[u] + peso;
                pai[v] = u;
            }
            ponteiro = ponteiro -> next;
        }
    }

    fprintf(qvertice, "Distâncias mínimas a partir do vértice %d:\n", vertx );
    for (int i = 0; i < n; i++) {
        fprintf(qvertice, "Vértice %d, Distância: %.2f, Caminho: ", i + 1, dist[i]);
        caminhominimo(pai, i , qvertice);
        fprintf(qvertice, "\n");
    }

    free(dist);
    free(VS);
    free(pai);
}

void Dijkstra_Matriz_Heap(float** matriz, int vertx, int n, FILE* qvertice) {
    float* dist = (float*)malloc(n * sizeof(float));
    int* pai = (int*)malloc(n * sizeof(int));
    int* VS = (int*)calloc(n, sizeof(int));
    MinHeap* minHeap = createMinHeap(n); // criando heap

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
        dist[i] = FLT_MAX;
        minHeap->array[i] = (MinHeapNode){i, dist[i]};
        minHeap->pos[i] = i; // mapeia a posição do nó no heap
    }

    dist[vertx - 1] = 0;
    minHeap->array[vertx - 1].key = 0;

    decreaseKey(minHeap, vertx - 1, dist[vertx - 1]);
    minHeap->size = n; 

    while (!isEmpty(minHeap)) {
        MinHeapNode minNode = extractMin(minHeap);
        int u = minNode.vertex;
        VS[u] = 1;


        for (int v = 0; v < n; v++) {
            if (!VS[v] && matriz[u][v] != FLT_MAX && dist[u] + matriz[u][v] < dist[v]) {
                dist[v] = dist[u] + matriz[u][v]; // Atualiza a distância
                pai[v] = u; // Atualiza o pai
                decreaseKey(minHeap, v, dist[v]); // Atualiza o nó na min-heap
            }
        }
    }

    fprintf(qvertice, "Distâncias mínimas a partir do vértice %d:\n", vertx);
    for (int i = 0; i < n; i++) {
        fprintf(qvertice, "Vértice %d, Distância: %.2f, Caminho: ", i + 1, dist[i]);
        caminhominimo(pai, i, qvertice); // Chama a função para imprimir o caminho mínimo
        fprintf(qvertice, "\n");
    }

    free(dist);
    free(pai);
    free(VS);
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);
}

void Dijkstra_Lista_Heap(Node* lista[], int vertx, int n, FILE* qvertice) {

    // Inicializa vetores de distância, pais e heap
    float* dist = (float*)malloc(n * sizeof(float));
    int* pai = (int*)malloc(n * sizeof(int));
    int* VS = (int*)calloc(n, sizeof(int)); 
    MinHeap* minHeap = createMinHeap(n);

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
        dist[i] = FLT_MAX;
        minHeap->array[i] = (MinHeapNode){i, dist[i]};
        minHeap->pos[i] = i;
    }

    dist[vertx - 1] = 0;
    minHeap->array[vertx - 1].key = 0;
    decreaseKey(minHeap, vertx - 1, dist[vertx - 1]);
    minHeap->size = n; // Define o tamanho do heap


    // Loop principal de Dijkstra
    while (!isEmpty(minHeap)) {
        MinHeapNode minNode = extractMin(minHeap);
        int u = minNode.vertex;
        VS[u] = 1;

        // Itera sobre os vizinhos de `u`
        Node* ponteiro = lista[u];
        while (ponteiro != NULL) {
            int v = ponteiro->vertex;
            float peso = ponteiro->peso;

            if (!VS[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pai[v] = u;
                decreaseKey(minHeap, v, dist[v]);
            }
            ponteiro = ponteiro->next;
        }
    }

    fprintf(qvertice, "Distâncias mínimas a partir do vértice %d:\n", vertx);
    for (int i = 0; i < n; i++) {
        fprintf(qvertice, "Vértice %d, Distância: %.2f, Caminho: ", i + 1, dist[i]);
        caminhominimo(pai, i, qvertice);
        fprintf(qvertice, "\n");
    }

    free(dist);
    free(pai);
    free(VS);
    free(minHeap->array);
    free(minHeap);
}

int main(void) {
    char arquivo[10000];
    char repr[3];
    printf("Digite o arquivo txt: ");
    fgets(arquivo, sizeof(arquivo), stdin);
    arquivo[strcspn(arquivo, "\n")] = 0; // Remove a nova linha
    int n;

    FILE *grafo = fopen(arquivo, "r");
    FILE *qvertice = fopen("resultados.txt", "wt");

    if (grafo == NULL) {
        printf("Erro ao abrir o arquivo de grafo.\n");
        return 1;
    }
    if (qvertice == NULL) {
        printf("Erro ao abrir o arquivo de resultados.\n");
        fclose(grafo);
            return 1;
    }

    printf("Escolha como deseja representar o grafo: Digite somente a letra [M] para Matriz ou [L] para Lista: ");
    fgets(repr, sizeof(repr), stdin);
    repr[strcspn(repr, "\n")] = 0;

    if (strcmp(repr, "M") == 0) { // MATRIZ ADJ
        int vertice;
        char busca[2];

        float** matrizadj = matrizpeso(grafo, &n, qvertice);

        printf("Escolha se deseja o Dijkstra com ou sem Heap: Digite [H] para Heap e [V] para Vetor: ");

        scanf("%s", busca);

        if (strcmp(busca, "H") == 0) { // Dijkstra_Matriz_Heap

            printf("Digite o vertice inicial para o Dijkstra: ");

            scanf("%d", &vertice);

            Dijkstra_Matriz_Heap(matrizadj, vertice, n, qvertice);   

        }

        if (strcmp(busca, "V") == 0){ // Dijkstra_Matriz

            printf("Digite o vertice inicial para a DFS: ");

            scanf("%d", &vertice);

            Dijkstra_Matriz(matrizadj, vertice, n, qvertice);        
        }

        for (int i = 0; i < n; i++) {
            free(matrizadj[i]);
        }
        free(matrizadj);
    } 

    else if (strcmp(repr, "L") == 0) { // LISTA ADJ
        char busca[2];
        int vertice;
        Node** listaadj = lista(grafo, &n, qvertice);

        printf("Escolha se deseja o Dijkstra com ou sem Heap: Digite [H] para Heap e [V] para Vetor: ");

        scanf("%s", busca);

        if (strcmp(busca, "H") == 0 || strcmp(busca, "h") == 0) { // Dijkstra_Lista_Heap

            printf("Digite o vértice inicial para o Dijkstra: ");

            scanf("%d", &vertice);

            Dijkstra_Lista_Heap(listaadj, vertice, n, qvertice);   

        }
        if (strcmp(busca, "V") == 0 || strcmp(busca, "v") == 0){

            printf("Digite o vértice inicial para o Dijkstra: "); // Dijkstra_Lista

            scanf("%d", &vertice);

            Dijkstra_Lista(listaadj, vertice, n, qvertice);            
        }
        

        for (int i = 0; i < n; i++) {    // Libere a memória depois de usar
            Node* temp = listaadj[i];
            while (temp) {
                Node* toDelete = temp;
                temp = temp->next;
                free(toDelete);
        
            }
        }
    }
    fclose(grafo);
    fclose(qvertice);

    return 0;
}