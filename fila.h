#ifndef FILA_H
#define FILA_H

typedef struct Fila {
    int *fila;
    int head;
    int tail;
    int tamanho;
    int atual;
} Fila;

Fila* criandofila(int atual);

void addfila(Fila* q,int valor);

int kickfila(Fila* q);

int kickDFS(Fila* q);

int Vazio(Fila* q);

void freeQueue(Fila* q);

void push(int* stack, int* top, int value);

int pop(int* stack, int* top);

int comparar(const void* a, const void* b);

#endif