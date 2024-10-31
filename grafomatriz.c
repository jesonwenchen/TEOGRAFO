#include "grafomatriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "fila.h"
#include <float.h>

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

void Encontrar_Componentes_conexa_Matriz(int** matriz, int n, FILE* qvertice){
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

void Distancia_Matriz(int**matriz, int n, int vertice1, int vertice2 , FILE* qvertice){
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
    int maiornivel = 0;
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
                if (nivel[i] > maiornivel) {
                    maiornivel = i;            
                }
            }
        }

    }
    return maiornivel; // vertice "mais afastado"
}

void AproxDiametro(int** matriz, int n, FILE* qvertice){
    srand(time(NULL));
    int vertice = primitivaBFS(matriz, n ,(rand() % n)); // pegando um vertice qualquer 
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
        for (int i = 0; i < n; i++){
            if (matriz[verticeatual][i] == 1 && !visitados[i]) { // não precisa explicar o obvio ne.
                visitados[i] = 1; // atualizando tudo...
                nivel[i] = nivel[verticeatual] + 1;
                addfila(fila, i); // aqui coloco esse vizinho na minha queue
                if (nivel[i] > maiornivel) {
                    maiornivel = nivel[i]; // catando a maior distancia           
                }
            }
        }
    }
    free(visitados);
    free(nivel);    
    fprintf(qvertice,"O valor aproximado do diâmetro é: %d \n",maiornivel);
}

void Diametro_Matriz(int**matriz, int n, FILE* qvertice){ // realizer BFS em TODOS os vértices.
    
    if (n < 100000){    
        int diametro = 0;
        for (int k = 0; k < n; k++){ // vertice que estou fazendo BFS

            int*visitados = (int*)calloc(n, sizeof(int));
            int*nivel = (int*)malloc(n * sizeof(int));
            Fila* fila = criandofila(n);
            addfila(fila, k);

            while(!Vazio(fila)){
                int verticefila = kickfila(fila);
                for (int i = 0; i < n; i++){
                    if (matriz[verticefila][i] == 1 && !visitados[i]) { // não precisa explicar o obvio ne.
                        visitados[i] = 1; // atualizando tudo...
                        nivel[i] = nivel[verticefila] + 1;
                        addfila(fila, i); // aqui coloco esse vizinho na minha queue
                        if (diametro < nivel[i]){ // to guardando sempre o maior valor.
                            diametro = nivel[i];
                        }
                    }
                }
            }
            free(visitados);
            free(nivel);
        } fprintf(qvertice, "O diâmetro do grafo é: %d \n", diametro);
    } else AproxDiametro(matriz, n, qvertice);
}

void Graus_Matriz(FILE* qvertice, int n, int** matrizadj){

    int grau_minimo = INT_MAX;
    int grau_max = INT_MIN;
    int* grauarray = (int*)malloc(n * sizeof(int));
    int media;

    for (int i = 0; i < n; i++) {
        int grau = 0;
        for (int j = 0; j < n; j++) {
            if (matrizadj[i][j] == 1) {
                grau++;
                grauarray[i]++;
                media += grau;
            }
        }
        if (grau > grau_max){
            grau_max = grau;
        }
        if (grau < grau_minimo) {
            grau_minimo = grau;
        }
    }

    int mediana;
    qsort(grauarray, n, sizeof(int), comparar);

    if (n % 2 != 0) mediana = grauarray[(n+1)/2];
    else mediana = (grauarray[n/2] + grauarray[n/2 - 1]) / 2.0;

    fprintf(qvertice, "O grau mínimo é: %d \n", grau_minimo);
    fprintf(qvertice, "O grau máximo é: %d \n", grau_max);
    fprintf(qvertice, "O número de vértices é: %d \n", n);
    fprintf(qvertice, "A mediana do grafo é: %d \n", mediana);
    fprintf(qvertice, "O grau medio do grafo é: %d \n", media / n);
}

int** Matriz_Sem_Peso(FILE*grafo, int* n, FILE* qvertice){
    if (grafo == NULL){
        printf("Erro ao abrir\n");
        return NULL;
    }

    if (fscanf(grafo, "%d", n) != 1){
        printf("Erro ao ler o número de vértices\n");
        return NULL;
    }

    int** matriz = (int**)malloc(*n * sizeof(int*));

    for (int i = 0; i < *n; i++) {
        matriz[i] = (int*)malloc(*n * sizeof(int));
        for (int j = 0; j < *n; j++){
            matriz[i][j] = 0;
        }
    }

    int m = 0;
    int i, j; 

    while(fscanf(grafo, "%d %d", &i, &j) == 2){ 
        m++;
        matriz[i - 1][j - 1] = 1;
        matriz[j - 1][i - 1] = 1;
    }

    fprintf(qvertice, "O número de arestas do grafo é %d: \n", m);
    return matriz;
}
