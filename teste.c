#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef struct {
    int *fila;
    int head;
    int tail;
    int tamanho;
    int atual;
} Fila;

typedef struct Node {// Estrutura de nó para a lista de adjacência
    int vertex;
    struct Node* next;
} Node;

void DFSrealComponentes(int** matriz, int* visitados, int vertice, int n, int* componentes, int compIndex, int* tamanhoComp) {
    visitados[vertice] = 1; // marcando como visitado
    componentes[vertice] = compIndex;  // marcando o vértice como pertencente ao componente atual
    tamanhoComp[compIndex]++;  // Incrementa o tamanho da componente
    for (int i = 0; i < n; i++) {
        if (matriz[vertice][i] == 1 && !visitados[i]) {
            DFSrealComponentes(matriz, visitados, i, n, componentes, compIndex, tamanhoComp); // DFS pois se é vizinho, faz parte da componente..
        }
    }
}

void eccc(int** matriz, int n, FILE* qvertice){
    int* visitados = (int*)calloc(n,sizeof(int)); // preenche com zeros
    int* componentes = (int*)malloc(n * sizeof(int));
    int* tamanhoC = (int*)calloc(n, sizeof(int)); // preenche com zeros; calloc
    int numcomp = 0;

    //achando os comp conexos

    for (int i =0; i < n; i++){
        if (!visitados[i]){
            DFSrealComponentes(matriz, visitados, i, n, componentes, numcomp, tamanhoC); // i = pode começar como zero,  não preciso fazer dps i-1.
            numcomp++;
        }
    }
    int* ordem = (int*)malloc(numcomp * sizeof(int));
    for (int i = 0; i < numcomp; i++) {
        ordem[i] = i;
    }

    // Bubble Sort
    for (int i = 0; i < numcomp - 1; i++) {
        for (int j = 0; j < numcomp - i - 1; j++) {
            if (tamanhoC[ordem[j]] < tamanhoC[ordem[j + 1]]) {
                int temp = ordem[j];
                ordem[j] = ordem[j + 1];
                ordem[j + 1] = temp;
            }
        }
    }
    // Imprimindo o resultado
    fprintf(qvertice, "Número de componentes conexos: %d\n", numcomp);
    for (int i = 0; i < numcomp; i++) {
        int compIndex = ordem[i];
        fprintf(qvertice, "Componente %d (Tamanho: %d): ", i + 1, tamanhoC[compIndex]);
        for (int j = 0; j < n; j++) {
            if (componentes[j] == compIndex) {
                fprintf(qvertice, "%d ", j + 1);
            }
        }
        fprintf(qvertice, "\n");
    }

    // Liberando a memória
    free(visitados);
    free(componentes);
    free(tamanhoC);
    free(ordem);

}

void DFScompLista(Node* lista[], int vertice, int visitados[], int componenteatual[], int numcomp, FILE* qvertice, int* tamanhoC){
    visitados[vertice] = 1;
    componenteatual[vertice] = numcomp;
    //fprintf(qvertice, "%d ", vertice + 1);
    Node* ponteiro = lista[vertice];
    tamanhoC[numcomp]++;
    
    while (ponteiro){
        if (!visitados[ponteiro->vertex]){
            DFScompLista(lista, ponteiro->vertex, visitados, componenteatual, numcomp, qvertice, tamanhoC);
        }
        ponteiro = ponteiro->next;
    }
}

void ecccLista(Node* lista[], int n, FILE* qvertice){
    int* visitados = (int*)calloc(n, sizeof(int)); // Vetor de visitados
    int* componenteAtual = (int*)malloc(n * sizeof(int)); // Vetor para armazenar as componentes
    int numComponentes = 0;  // Contador de componentes conexas
    int* tamanhoC = (int*)calloc(n, sizeof(int)); // preenche com zeros; calloc


    for (int i = 0; i < n; i++) {
        if (!visitados[i]) {

            DFScompLista(lista, i, visitados, componenteAtual, numComponentes, qvertice, tamanhoC);
            numComponentes++;  // Nova componente encontrada

            //fprintf(qvertice, "Componente %d: ", numComponentes);

            // Chama DFS para encontrar todos os vértices da componente

            //fprintf(qvertice, "\n");  // Nova linha após imprimir a componente
        }
    }
    int* ordem = (int*)malloc(numComponentes * sizeof(int));
    for (int i = 0; i < numComponentes; i++) ordem[i] = i;

    for (int i = 0; i < numComponentes - 1; i++) {
        for (int j = 0; j < numComponentes - i - 1; j++) {
            if (tamanhoC[ordem[j]] < tamanhoC[ordem[j + 1]]) {
                int temp = ordem[j];
                ordem[j] = ordem[j + 1];
                ordem[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < numComponentes; i++) {
        int compIndex = ordem[i];
        fprintf(qvertice, "Componente %d (Tamanho: %d): ", i + 1, tamanhoC[compIndex]);
        for (int j = 0; j < n; j++) {
            if (componenteAtual[j] == compIndex) {
                fprintf(qvertice, "%d ", j + 1);
            }
        }
        fprintf(qvertice, "\n");
    }

    fprintf(qvertice, "Componentes Conexas:%d\n", numComponentes);
    free(visitados);
    free(componenteAtual);
}

Fila* criandofila(int atual){
    Fila* q = (Fila*)malloc(sizeof(Fila));
    q->fila = (int*)malloc(atual * sizeof(int));
    q->tail = -1;
    q->head = -1;
    q->tamanho = 0;
    q->atual = atual;
    return q;
}

void addfila(Fila* q,int valor) { // funcao que add um elemento na fila.
    if (q->tamanho == q->atual){
        q->atual *= 2;
        q->fila = (int*)realloc(q->fila, q->atual * sizeof(int));
    }
    if (q->head == -1) q->head = 0; // fila vazia.
    q->tail = (q->tail +1 ) % q->atual; // evita que o indice do elemento tail fique maior do que o tamanho.
    q->fila[q->tail] = valor; // atualiza na fila o valor que veio no parametro
    q->tamanho++; // aumenta o tamanho da fila. 
    
}

int kickfila(Fila* q) { // retira o head
    if (q->tamanho == 0){
        printf("Fila vazia\n");
        return -1;
    }
    int valor = q->fila[q->head];
    q->head = (q->head + 1) % q->atual;
    q->tamanho--;
    return valor;
}

int kickDFS(Fila* q) {
    if (q->tamanho == 0){
        printf("fila vazia");
        return -1;
    }
    int valor = q->fila[q->tail];
    q->tail = (q->tail - 1);
    q->tamanho--;
    return valor;
}

int Vazio(Fila* q){
    return q->tamanho == 0;
}

void freeQueue(Fila* q) {
    free(q->fila);
    free(q);
}

void DFSreal(int** matriz, int* visitados, int* pai, int*nivel, int vertice, int n, int nivelreal){
    visitados[vertice] = 1;
    nivel[vertice] = nivelreal;
    for (int i = 0; i < n; i++) {
        if (matriz[vertice][i] == 1 && !visitados[i]) {
            pai[i] = vertice; // atualizando o pai do vértice
            DFSreal(matriz, visitados, pai, nivel, i, n, nivelreal + 1);
        }
    }
}

void DFS(int**matriz, int n, int inicialvertx, FILE* qvertice){
    int* visitados = (int*)calloc(n, sizeof(int)); // tudo com zeros
    int* pai = (int*)malloc(n * sizeof(int));
    int* nivel = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        pai[i] = -1;
        nivel[i] = -1;
    } // ate aqui igual.

    DFSreal(matriz, visitados, pai, nivel, inicialvertx - 1, n, 0);

    fprintf(qvertice, "vertice             pai            nivel \n");
    for (int i = 0; i < n; i++) {
        fprintf(qvertice, "     %d               %d             %d\n", i + 1, pai[i] + 1, nivel[i]);
    }

    free(visitados);
    free(pai);
    free(nivel);
}

void BFS(int** matriz, int n, int inicialvertx, FILE* qvertice){
    int* visitados = (int*)calloc(n,sizeof(int)); // vetor de visitados
    int* pai = (int*)malloc(n * sizeof(int)); // vetor de paix
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis

    for (int i = 0; i < n; i++){
        pai[i] = -1;
        nivel[i] = -1;
    }

    Fila* fila = criandofila(n); // criando uma fila 
    visitados[inicialvertx - 1] = 1; // marcando como visitado o vertice inicial
    nivel[inicialvertx - 1] = 0; //raiz nivel zero
    addfila(fila, inicialvertx - 1); // colocando esse vertice na fila.

    while (!Vazio(fila)) { // enquanto a fila nao ficar vazia rola...
        int verticeatual = kickfila(fila); // primeiro a sair da fila, vulgo head.

        for (int i = 0; i < n; i++){
            if (matriz[verticeatual][i] == 1 && !visitados[i]) { // não precisa explicar o obvio ne.
                visitados[i] = 1; // atualizando tudo...
                pai[i] = verticeatual;
                nivel[i] = nivel[verticeatual] + 1;
                addfila(fila, i); // aqui coloco esse vizinho na minha queue
            }
        }

    }

    fprintf(qvertice, "vertice             pai            nivel \n");
    for (int i = 0; i < n; i++){ 
        fprintf(qvertice,"     %d               %d             %d\n", i+1, pai[i] + 1, nivel[i]);
    }
    free(visitados);
    free(pai);
    free(nivel);
    freeQueue(fila);

}

void BFSL(Node* lista[], int n, int inicialvertx, FILE* qvertice){
    int* visitados = (int*)calloc(n,sizeof(int)); // vetor de visitados
    int* pai = (int*)malloc(n * sizeof(int)); // vetor de paix
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis

    for (int i = 0; i < n; i++){
        pai[i] = -1;
        nivel[i] = -1;
    }
    Fila* fila = criandofila(n); // criando uma fila 
    visitados[inicialvertx - 1] = 1; // marcando como visitado o vertice inicial
    nivel[inicialvertx - 1] = 0; //raiz nivel zero
    addfila(fila, inicialvertx - 1); // colocando esse vertice na fila.
    
    while (!Vazio(fila)) { 
        int verticeatual = kickfila(fila); // retirando o proximo, head
        Node* ponteiro = lista[verticeatual]; // criando um ponteiro node, para apontar para lista adj do vertice atual

        while (ponteiro){ // até ficar com valor NULL
            int vv = ponteiro->vertex; // assign o valor do vertice considerado com vizinho
            
            if (!visitados[vv]){
                visitados[vv] = 1;
                nivel[vv] = nivel[verticeatual] + 1;
                pai[vv] = verticeatual;
                addfila(fila, vv);
            }
            ponteiro = ponteiro->next; // indo para o proximo nó da lista do vertice atual.
        }
    }
    fprintf(qvertice, "vertice             pai            nivel \n");
    for (int i = 0; i < n; i++){ 
        fprintf(qvertice,"     %d               %d             %d\n", i+1, pai[i] + 1, nivel[i]);
    }
    free(visitados);
    free(pai);
    free(nivel);
    freeQueue(fila);

}

void DFSrealL(Node* lista[], int* visitados, int* pai, int*nivel, int vertice, int nivelreal){
    visitados[vertice] = 1;
    nivel[vertice] = nivelreal;
    Node* ponteiro = lista[vertice];

    while (ponteiro){
        int vv = ponteiro->vertex;
        if (!visitados[vv]){
            visitados[vv] = 1;
            pai[vv] = vertice;
            DFSrealL(lista, visitados, pai, nivel, vv, nivelreal + 1);
        }
        ponteiro = ponteiro->next;
    }
}

void DFSL(Node* lista[], int vertice, int n, FILE* qvertice){
    int* visitados = (int*)calloc(n,sizeof(int)); // vetor de visitados
    int* pai = (int*)malloc(n * sizeof(int)); // vetor de paix
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis

    for (int i = 0; i < n; i++){
        pai[i] = -1;
        nivel[i] = -1;
    }

    DFSrealL(lista, visitados, pai, nivel, vertice - 1, 0);

    fprintf(qvertice, "vertice             pai            nivel \n");
    for (int i = 0; i < n; i++){ 
        fprintf(qvertice,"     %d               %d             %d\n", i+1, pai[i] + 1, nivel[i]);
    }
    free(visitados);
    free(pai);
    free(nivel);

}

/*void printAdjList(Node* adjList[], int n) { // Função para imprimir a lista de adjacência
    for (int i = 0; i < n; i++) {
        Node* temp = adjList[i];
        printf("Vertice %d: ", i + 1); // Adiciona 1 para mostrar vértices como 1-indexados
        while (temp) {
            printf("%d -> ", temp->vertex + 1);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}*/

void addEdgeToAdjList(Node** adjList, int src, int dest) {// Função para adicionar um vértice à lista de adjacência
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    Node* current = adjList[src];
    while (current != NULL) {
        if (current->vertex == dest) {
            // Se o vértice já existe na lista, sai da função
            return;
        }
        current = current->next;
    }
    newNode->next = adjList[src];
    adjList[src] = newNode;
}

/*void calculaMemoriaMatriz(int n) {
    size_t memoria = (n * n * sizeof(int)); // memória usada pela matriz
    printf("Memoria usada pela matriz de adjacencia: %u MB\n", memoria/(1024*1024));
}*/

/*void calculaMemoriaLista(int n, int numArestas) {
    size_t memoriaLista = n * sizeof(Node*) + 2 * numArestas * sizeof(Node);
    printf("Memoria usada pela lista de adjacencia: %d MB\n", memoriaLista / (1024*1024));
}*/

void distanciaL(Node* lista[], int n, int v1, int v2, FILE* qvertice){ // fazer um "BFS " usando v1.
    int*visitados = (int*)calloc(n, sizeof(int));
    int*nivel = (int*)malloc(n * sizeof(int));
    int distancia = - 1;

    for (int i = 0; i < n; i++){
        nivel[i] = -1;
    }
    Fila* fila = criandofila(n);
    visitados[v1 - 1] = 1;
    nivel[v1 - 1] = 0;
    addfila(fila, v1 - 1);

    while(!Vazio(fila)) {
        int verticeatual = kickfila(fila);
        Node* ponteiro = lista[verticeatual];

        while (ponteiro) {
            int v = ponteiro->vertex;
            if (!visitados[v]){
                visitados[v] = 1;
                nivel[v] = nivel[verticeatual] + 1;

                if (v == v2 - 1) {
                    distancia = nivel[v];
                    break;
                }
                addfila(fila, v);
            }
            ponteiro = ponteiro->next;
        }
        if (distancia != -1) break;
    }
    fprintf(qvertice, "A distância entre os vértices é %d \n", distancia);

    free(visitados);
    free(nivel);
}

int primitivaBFSL(Node* lista[], int n, int inicialvertx){
    int* visitados = (int*)calloc(n,sizeof(int)); // vetor de visitados
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis
    int a;
    int maiornivel = 0;
    for (int i = 0; i < n; i++) nivel[i] = -1;

    Fila* fila = criandofila(n); // criando uma fila 
    visitados[inicialvertx - 1] = 1; // marcando como visitado o vertice inicial
    nivel[inicialvertx - 1] = 0; //raiz nivel zero
    addfila(fila, inicialvertx - 1); // colocando esse vertice na fila.
    
    while (!Vazio(fila)) { 
        int verticeatual = kickfila(fila); // retirando o proximo, head
        Node* ponteiro = lista[verticeatual]; // criando um ponteiro node, para apontar para lista adj do vertice atual

        while (ponteiro){ // até ficar com valor NULL
            int vv = ponteiro->vertex; // assign o valor do vertice considerado com vizinho
            
            if (!visitados[vv]){
                visitados[vv] = 1;
                nivel[vv] = nivel[verticeatual] + 1;
                addfila(fila, vv);
            }
            ponteiro = ponteiro->next; // indo para o proximo nó da lista do vertice atual.
        }
    }
    for (int i = 0; i < n; i++){
        if (nivel[i] == -1) return -1;
        if (nivel[i] > maiornivel){
            maiornivel = nivel[i];
            a = i;
        }
    }
    return a;

    free(visitados);
    free(nivel);
    freeQueue(fila);

}

void diametroL(Node* lista[], int n, int inicial, FILE* qvertice){
    int vertice = primitivaBFSL(lista, n , inicial);

    if (vertice == -1){
        fprintf(qvertice,"não existe diametro, grafo NAO conexo. \n");
    } else {
        int* visitados = (int*)calloc(n,sizeof(int)); // vetor de visitados
        int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis
        for (int i = 0; i < n; i++) nivel[i] = -1;

        Fila* fila = criandofila(n); // criando uma fila 
        visitados[vertice] = 1; // marcando como visitado o vertice inicial
        nivel[vertice] = 0; //raiz nivel zero
        addfila(fila, vertice); // colocando esse vertice na fila.
        
        while (!Vazio(fila)) { 
            int verticeatual = kickfila(fila); // retirando o proximo, head
            Node* ponteiro = lista[verticeatual]; // criando um ponteiro node, para apontar para lista adj do vertice atual

            while (ponteiro){ // até ficar com valor NULL
                int vv = ponteiro->vertex; // assign o valor do vertice considerado com vizinho
                
                if (!visitados[vv]){
                    visitados[vv] = 1;
                    nivel[vv] = nivel[verticeatual] + 1;
                    addfila(fila, vv);
                }
                ponteiro = ponteiro->next; // indo para o proximo nó da lista do vertice atual.
            }
        }
        int maiornivel = 0;
        for (int i = 0; i < n; i++){
            if (nivel[i] > maiornivel){
                maiornivel = nivel[i];
            }
        }    
        fprintf(qvertice, "o diametro desse grafo é %d \n", maiornivel);
    }
}

void distancia(int**matriz, int n, int vertice1, int vertice2 , FILE* qvertice){
    int* visitados = (int*)calloc(n,sizeof(int)); // vetor de visitados
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis
    int distancia = -1;
    for (int i = 0; i < n; i++){
        nivel[i] = -1;
    }
    Fila* fila = criandofila(n); // criando uma fila 
    visitados[vertice1 - 1] = 1; // marcando como visitado o vertice inicial
    nivel[vertice1 - 1] = 0; //raiz nivel zero
    addfila(fila, vertice1 - 1); // colocando esse vertice na fila.

    while (!Vazio(fila)) { // enquanto a fila nao ficar vazia rola...
        int verticeatual = kickfila(fila); // primeiro a sair da fila, vulgo head.

        for (int i = 0; i < n; i++){
            if (matriz[verticeatual][i] == 1 && !visitados[i]) { // não precisa explicar o obvio ne.
                visitados[i] = 1; // atualizando tudo...
                nivel[i] = nivel[verticeatual] + 1;
                addfila(fila, i); // aqui coloco esse vizinho na minha queue
                if (i == vertice2 - 1){
                    distancia = nivel[i];
                    break;
                }
            }
            if (i == vertice2 - 1){
                break;
            }
        }
        if (distancia != -1) break;
    }
    fprintf(qvertice, "A distância entre os vértices é %d \n", distancia);

    free(visitados);
    free(nivel);

}

int primitivaBFS(int**matriz, int n, int inicialvertx){
    int* visitados = (int*)calloc(n, sizeof(int));
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis

    for (int i = 0; i < n; i++){
        nivel[i] = -1;
    }

    Fila* fila = criandofila(n); // criando uma fila 
    visitados[inicialvertx - 1] = 1; // marcando como visitado o vertice inicial
    nivel[inicialvertx - 1] = 0; //raiz nivel zero
    addfila(fila, inicialvertx - 1); // colocando esse vertice na fila.

    while (!Vazio(fila)) { // enquanto a fila nao ficar vazia rola...
        int verticeatual = kickfila(fila); // primeiro a sair da fila, vulgo head.
        for (int i = 0; i < n; i++){
            if (matriz[verticeatual][i] == 1 && !visitados[i]) { // não precisa explicar o obvio ne.
                visitados[i] = 1; // atualizando tudo...
                nivel[i] = nivel[verticeatual] + 1;
                addfila(fila, i); // aqui coloco esse vizinho na minha queue
            }
        }

    }
    int maiornivel = 0;
    int a;
    for (int v = 0; v < n; v++){
        if (nivel[v] == -1) return -1;
        if (nivel[v] > maiornivel){
            maiornivel = nivel[v];
            a = v;
        }
    } return a; }

void diametro(int**matriz, int n, int inicialvertx , FILE* qvertice){
    int vertice = primitivaBFS(matriz, n, inicialvertx); // aqui retorna o valor a;

    if (vertice == -1){
        fprintf(qvertice, "não existe diametro, grafo NAO conexo. \n");
    } else {
        int* visitados = (int*)calloc(n, sizeof(int));
        int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis
        // agora tenho que fazer um BFS com esse vertice "a". e achar o maior nivel nesse BFS, ai será o valor do meu DIAMETRO.
        for (int i = 0; i < n; i++){
            nivel[i] = -1;
        }

        Fila* fila = criandofila(n); // criando uma fila 
        visitados[vertice] = 1; // marcando como visitado o vertice inicial
        nivel[vertice] = 0; //raiz nivel zero
        addfila(fila, vertice); // colocando esse vertice na fila.

        while (!Vazio(fila)) { // enquanto a fila nao ficar vazia rola...
            int verticeatual = kickfila(fila); // primeiro a sair da fila, vulgo head.

            for (int i = 0; i < n; i++){
                if (matriz[verticeatual][i] == 1 && !visitados[i]) { // não precisa explicar o obvio ne.
                    visitados[i] = 1; // atualizando tudo...
                    nivel[i] = nivel[verticeatual] + 1;
                    addfila(fila, i); // aqui coloco esse vizinho na minha queue
                }
            }
        }
        int maiornivel = 0;
        for (int v = 0; v < n; v++){
            if (nivel[v] > maiornivel){
                maiornivel = nivel[v];
            }
        }
        fprintf(qvertice, "o Diametro desse grafo é %d \n", maiornivel);
    }
}
int main(void) {

    char arquivo[10000];
    char repr[2];
    printf("Digite o arquivo txt: ");
    fgets(arquivo, sizeof(arquivo), stdin);
    arquivo[strcspn(arquivo, "\n")] = 0;
    FILE *grafo;
    FILE *qvertice;
    int m = 0;
    int x, y, n;
    // Abrindo os arquivos txts
    grafo = fopen(arquivo, "r");
    qvertice = fopen("resultados.txt", "wt");

    clock_t start_time, end_time; // Para medir o tempo
    double cpu_time_used;
    start_time = clock(); // Inicia a contagem do tempo

    if (grafo == NULL || qvertice == NULL) { // verificando possíveis erros...
        printf("Erro ao abrir\n");
        return 1;
    }

    if (fscanf(grafo, "%d", &n) != 1) {
        printf("Erro ao ler o número de vértices\n");
        return 1;
    }

    printf("Escolha como deseja representar o grafo: Digite somente a letra [M] para Matriz ou [L] para Lista: ");
    fgets(repr, sizeof(repr), stdin);
    repr[strcspn(repr, "\n")] = 0;

    if (strcmp(repr, "M") == 0) { // MATRIZ ADJ
        int vertice;
        char busca[2];

        int** matrizadj = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            matrizadj[i] = (int*)calloc(n, sizeof(int)); // Inicializa com 0
        }
        while (fscanf(grafo, "%d %d", &x, &y) == 2) {
            m++;
            matrizadj[x-1][y-1] = 1; // Preenchendo a matriz de adjacência
            matrizadj[y-1][x-1] = 1; // Para grafos não direcionados
        }

        printf("Escolha qual busca deseja: Digite [B] para BFS e [D] para DFS: ");
        scanf("%s", busca);

        if (strcmp(busca, "B") == 0) {

            printf("Digite o vertice inicial para a BFS: ");

            scanf("%d", &vertice);

            BFS(matrizadj, n, vertice, qvertice);   

        }
        if (strcmp(busca, "D") == 0){

            printf("Digite o vertice inicial para a DFS: ");

            scanf("%d", &vertice);

            DFS(matrizadj, n, vertice, qvertice);            
        }
        /*fprintf(qvertice, "Matriz de Adjacência:\n");
        for (int i = 0; i < n; i++) {  
            for (int j = 0; j < n; j++) {
                fprintf(qvertice, "%d ", matrizadj[i][j]);
            }
            fprintf(qvertice, "\n");
        }*/

        int grau_minimo = INT_MAX;
        int grau_max = INT_MIN;
        for (int i = 0; i < n; i++) {
            int grau = 0;
            for (int j = 0; j < n; j++) {
                if (matrizadj[i][j] == 1) {
                    grau++;
                }
            }
            if (grau > grau_max){
                grau_max = grau;
            }
            if (grau < grau_minimo) {
                grau_minimo = grau;
            }
        }


        eccc(matrizadj, n, qvertice);

        fprintf(qvertice, "O grau mínimo é: %d\n", grau_minimo);
        fprintf(qvertice, "O grau máximo é: %d\n", grau_max);
        fprintf(qvertice, "O número de vértices é: %d\n", n);

        diametro(matrizadj, n, 1, qvertice);


        for (int i = 0; i < n; i++) {
            free(matrizadj[i]);
        }
        free(matrizadj);

    } else if (strcmp(repr, "L") == 0) { // LISTA ADJ
        char busca[2];
        int vertice;
        Node* adjList[n];
        for (int i = 0; i < n; i++) {
            adjList[i] = NULL;  // inicializando as listas como NULL
        }
        while (fscanf(grafo, "%d %d", &x, &y) == 2) {
            m++;
            addEdgeToAdjList(adjList, x - 1, y - 1);// add aresta à lista de adjacência
            addEdgeToAdjList(adjList, y - 1, x - 1);
        }

        printf("Escolha qual busca deseja: Digite [B] para BFS e [D] para DFS: ");

        scanf("%s", &busca);

        if (strcmp(busca, "B") == 0) {

            printf("Digite o vértice inicial para a BFS: ");

            scanf("%d", &vertice);

            BFSL(adjList, n, vertice, qvertice);   

        }
        if (strcmp(busca, "D") == 0){

            printf("Digite o vértice inicial para a DFS: ");

            scanf("%d", &vertice);

            DFSL(adjList, vertice, n, qvertice);            
        }

        int grau_minimo = INT_MAX;
        int grau_max = INT_MIN;
        // Calculando o grau de cada vértice
        for (int i = 0; i < n; i++) {
            int grau = 0;
            Node* temp = adjList[i];
            while (temp != NULL) {
                grau++;
                temp = temp->next;
            }
            if (grau > grau_max){
                grau_max = grau;
            }
            if (grau < grau_minimo) {
                grau_minimo = grau;
            }
        }
        ecccLista(adjList, n, qvertice);   


        fprintf(qvertice, "O grau mínimo é: %d\n", grau_minimo);
        fprintf(qvertice, "O grau máximo é: %d\n", grau_max);
        fprintf(qvertice, "O número de vértices é: %d\n", n);
        diametroL(adjList, n, 1, qvertice);

        for (int i = 0; i < n; i++) {    // Libere a memória depois de usar
        Node* temp = adjList[i];
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);
    
        }
        }
    }

    fprintf(qvertice, "O número de arestas desse grafo é: %d\n", m); // número de arestas no arquivo de saída

    end_time = clock(); // Termina a contagem do tempog
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo em segundos
    fprintf(qvertice, "Tempo de execucao: %f segundos\n", cpu_time_used);

    //calculaMemoriaMatriz(n);
    //calculaMemoriaLista(n, m);

    // Fechando os arquivos
    fclose(qvertice);
    fclose(grafo);


    return 0;
}


