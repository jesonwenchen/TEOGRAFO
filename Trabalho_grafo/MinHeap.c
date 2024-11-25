#include "Minheap.h"
#include <stdio.h>
#include <stdbool.h>
#include <float.h>

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

void caminhominimo(int* pai, int vAtual, FILE* qvertice) {
    if (pai[vAtual] == -1){
        fprintf(qvertice,"%d",vAtual + 1);
        return; // para nao continuar mais
    }
    caminhominimo(pai, pai[vAtual], qvertice);
    fprintf(qvertice, " -> %d", vAtual + 1);
    
}


