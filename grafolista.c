#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include "grafolista.h"
#include "fila.h"

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

void Encontrar_Componentes_conexas_Lista(Node* lista[], int n, FILE* qvertice){
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

void DFS_NaoRecursiva(Node* lista[], int* visitados, int* pai, int* nivel, int vertice, int n) {
    int* stack = (int*)malloc(n * sizeof(int)); 
    int top = -1; 
    int nivel_atual = 0;

 
    push(stack, &top, vertice);
    nivel[vertice] = nivel_atual;
    visitados[vertice] = 1;

    while (top != -1) { 
        int atual = pop(stack, &top);
        Node* ponteiro = lista[atual];

        while (ponteiro) {
            int vv = ponteiro->vertex;
            if (!visitados[vv]) {
                push(stack, &top, vv); 
                visitados[vv] = 1;
                pai[vv] = atual;
                nivel[vv] = nivel[atual] + 1; 
            }
            ponteiro = ponteiro->next;
        }
    }

    free(stack);
}

void DFSL_NaoRecursiva(Node* lista[], int vertice, int n, FILE* qvertice) {
    if (qvertice == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int* visitados = (int*)calloc(n, sizeof(int)); // Vetor de visitados
    int* pai = (int*)malloc(n * sizeof(int)); // Vetor de pais
    int* nivel = (int*)malloc(n * sizeof(int)); // Vetor de níveis

    for (int i = 0; i < n; i++) {
        pai[i] = -1; // Inicializa todos os pais como -1 (sem pai)
        nivel[i] = -1; // Inicializa os níveis como -1 (não visitado)
    }

    DFS_NaoRecursiva(lista, visitados, pai, nivel, vertice - 1, n);

    // Grava os resultados no arquivo
    fprintf(qvertice, "Vertice             Pai            Nivel \n");
    for (int i = 0; i < n; i++) {
        fprintf(qvertice, "     %d               %d             %d\n", i + 1, pai[i] + 1, nivel[i]);
    }

    free(visitados);
    free(pai);
    free(nivel);
}

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
    int maiornivel = 0;
    for (int i = 0; i < n; i++) nivel[i] = -1;

    Fila* fila = criandofila(n); // criando uma fila 
    visitados[inicialvertx - 1] = 1; // marcando como visitado o vertice inicial
    nivel[inicialvertx - 1] = 0; //raiz nivel zero
    addfila(fila, inicialvertx - 1); // colocando esse vertice na fila.
    
    while (!Vazio(fila)) { 
        int maiornivel = 0;
        int verticeatual = kickfila(fila); // retirando o proximo, head
        Node* ponteiro = lista[verticeatual]; // criando um ponteiro node, para apontar para lista adj do vertice atual

        while (ponteiro){ // até ficar com valor NULL
            int vv = ponteiro->vertex; // assign o valor do vertice considerado com vizinho
            if (!visitados[vv]){
                visitados[vv] = 1;
                nivel[vv] = nivel[verticeatual] + 1;
                addfila(fila, vv);
                if (nivel[vv] > maiornivel) maiornivel = vv;
            }
            ponteiro = ponteiro->next; // indo para o proximo nó da lista do vertice atual.
        }
    }
    return maiornivel;

    free(visitados);
    free(nivel);
    freeQueue(fila);
}

void AproxDiametroL(Node* lista[], int n, FILE* qvertice){
    srand(time(NULL));
    int vertice = primitivaBFSL(lista, n ,(rand() % n)); // pegando um vertice qualquer 
    int* visitados = (int*)calloc(n, sizeof(int));
    int* nivel = (int*)malloc(n * sizeof(int)); // vetor de niveis
    int maiornivel = 0;
    for (int i = 0; i < n; i++) nivel[i] = -1;

    Fila* fila = criandofila(n); // criando uma fila 
    visitados[vertice] = 1; // marcando como visitado o vertice inicial
    nivel[vertice] = 0; //raiz nivel zero
    addfila(fila, vertice); // colocando esse vertice na fila.

    while (!Vazio(fila)) { // enquanto a fila nao ficar vazia rola...
        int verticeatual = kickfila(fila); // primeiro a sair da fila, vulgo head.
        Node* ponteiro = lista[verticeatual];
        while (ponteiro) {
            int outrovertice = ponteiro->vertex;
            if (!visitados[outrovertice]) { // não precisa explicar o obvio ne.
                visitados[outrovertice] = 1; // atualizando tudo...
                nivel[outrovertice] = nivel[verticeatual] + 1;
                addfila(fila, outrovertice); // aqui coloco esse vizinho na minha queue
                if (nivel[outrovertice] > maiornivel) maiornivel = nivel[outrovertice];         
            }
        }
        ponteiro = ponteiro->next;
    }
    free(visitados);
    free(nivel);    
    fprintf(qvertice,"O valor aproximado do diâmetro é: %d \n",maiornivel);    
}

void diametroL(Node* lista[], int n, FILE* qvertice){
    if (n < 100000){    
        int diametro = 0;
        for (int k = 0; k < n; k++){
            int*visitados = (int*)calloc(n, sizeof(int));
            int*nivel = (int*)malloc(n *sizeof(int));
            for (int i = 0; i < n; i++) nivel[i] = -1;

            visitados[k] = 1;
            nivel[k] = 0;
            Fila* fila = criandofila(n);
            addfila(fila, k);

            while(!Vazio(fila)) {
                int vertice = kickfila(fila);
                Node* ponteiro = lista[vertice];

                while(ponteiro){
                    int outrovertice = ponteiro -> vertex;
                    if (!visitados[outrovertice]){
                        visitados[outrovertice] = 1;
                        nivel[outrovertice] = nivel[vertice] + 1;
                        addfila(fila, outrovertice);
                        if (diametro < nivel[outrovertice]) diametro = nivel[outrovertice];
                    }
                    ponteiro= ponteiro->next;
                }
            }
            free(visitados);
            free(nivel);
        } fprintf(qvertice, "O diâmetro do grafo é: %d \n", diametro);
    } else AproxDiametroL(lista, n, qvertice);
}

void Criando_Lista_Sem_Peso(Node** Lista, int inicial, int final){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = final;
    Node* current = Lista[inicial];
    while (current != NULL) {
        if (current->vertex == final) {
            // Se o vértice já existe na lista, sai da função
            return;
        }
        current = current->next;
    }
    newNode->next = Lista[inicial];
    Lista[inicial] = newNode;
}

Node** Lista_Sem_Peso(FILE* grafo, int* n, FILE* qvertice){
    int m = 0;
    int inicial, final;
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
    while (fscanf(grafo, "%d %d", &inicial, &final) == 2) {
        m++;
        Criando_Lista_Sem_Peso(lista, inicial - 1, final - 1);// add aresta à lista de adjacência
        Criando_Lista_Sem_Peso(lista, final - 1, inicial - 1);
    }
    fprintf(qvertice, "O número de arestas do grafo é %d \n", m);
    return lista;

}

void Graus_Lista(FILE* qvertice, int n, Node* listaadj[]){
    int grau_minimo = INT_MAX;
    int grau_max = INT_MIN;
    int* arraygrau = (int*)malloc(n * sizeof(int));
    int media = 0;
    int mediana;

    for (int i = 0; i < n; i++) {
        int grau = 0;
        Node* temp = listaadj[i];
        while (temp != NULL) {
            grau++;
            temp = temp->next;
        }
        arraygrau[i] = grau;
        media += grau;            
        if (grau > grau_max){
            grau_max = grau;
        }
        if (grau < grau_minimo) {
            grau_minimo = grau;
        }
    }
    media /= n;

    qsort(arraygrau, n, sizeof(int), comparar);
    if (n % 2 != 0 ) mediana = arraygrau[(n+1)/2];
    else mediana = (arraygrau[(n/2) + 1] + arraygrau[n/2])/2;
    fprintf(qvertice, "O grau mínimo é: %d\n", grau_minimo);
    fprintf(qvertice, "O grau máximo é: %d\n", grau_max);
    fprintf(qvertice, "A mediana do grafo é: %d\n", mediana);
    fprintf(qvertice, "O grau medio do grafo é: %d\n", media);   
    fprintf(qvertice, "O número de vértices é: %d\n", n);
}
