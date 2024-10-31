#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "fila.h"


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

void push(int* stack, int* top, int value) {
    stack[++(*top)] = value;
}

int pop(int* stack, int* top) {
    return stack[(*top)--];
}

int comparar(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}