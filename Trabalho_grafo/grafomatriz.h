#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>


void DFSrealComponentes(int** matriz, int* visitados, int vertice, int n, int* componentes, int compIndex, int* tamanhoComp);

void Encontrar_Componentes_conexa_Matriz(int** matriz, int n, FILE* qvertice);

void DFSreal(int** matriz, int* visitados, int* pai, int*nivel, int vertice, int n, int nivelreal);

void DFS(int**matriz, int n, int inicialvertx, FILE* qvertice);

void BFS(int** matriz, int n, int inicialvertx, FILE* qvertice);

void Distancia_Matriz(int**matriz, int n, int vertice1, int vertice2 , FILE* qvertice);

int primitivaBFS(int**matriz, int n, int inicialvertx);

void AproxDiametro(int** matriz, int n, FILE* qvertice);

void Diametro_Matriz(int**matriz, int n, FILE* qvertice);

void Graus_Matriz(FILE* qvertice, int n, int** matrizadj);

int** Matriz_Sem_Peso(FILE*grafo, int* n, FILE* qvertice);

#endif