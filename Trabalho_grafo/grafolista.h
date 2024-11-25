#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include <stdio.h>

typedef struct Node {// Estrutura de nó para a lista de adjacência
    int vertex;
    float peso;
    struct Node* next;
} Node;

void DFScompLista(Node* lista[], int vertice, int visitados[], int componenteatual[], int numcomp, FILE* qvertice, int* tamanhoC);

void Encontrar_Componentes_conexas_Lista(Node* lista[], int n, FILE* qvertice);

void BFSL(Node* lista[], int n, int inicialvertx, FILE* qvertice);

void DFS_NaoRecursiva(Node* lista[], int* visitados, int* pai, int* nivel, int vertice, int n);

void DFSL_NaoRecursiva(Node* lista[], int vertice, int n, FILE* qvertice);

void Criando_Lista_Sem_Peso(Node** Lista, int inicial, int final);

void distanciaL(Node* lista[], int n, int v1, int v2, FILE* qvertice);

int primitivaBFSL(Node* lista[], int n, int inicialvertx);

void AproxDiametroL(Node* lista[], int n, FILE* qvertice);

void diametroL(Node* lista[], int n, FILE* qvertice);

Node** Lista_Sem_Peso(FILE* grafo, int* n, FILE* qvertice);

void Graus_Lista(FILE* qvertice, int n, Node* listaadj[]);

void criando_lista_peso(Node** adjList, int a, int b, float peso);

Node** lista_peso(FILE*grafo, int* n, FILE* qvertice);

void Dijkstra_Lista(Node* lista[], int vertx, int n, FILE*qvertice);

void Dijkstra_Lista_Heap(Node* lista[], int vertx, int n, FILE* qvertice);




#endif
