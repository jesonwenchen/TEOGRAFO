#ifndef MINHEAP
#define MINHEAP

#include <stdbool.h>

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

MinHeap* createMinHeap(int capacity);

void swapMinHeapNode(MinHeapNode* a, MinHeapNode* b);

void minHeapify(MinHeap* minHeap, int idx);

MinHeapNode extractMin(MinHeap* minHeap);

void decreaseKey(MinHeap* minHeap, int vertex, float key);

bool isEmpty(MinHeap* minHeap);

#endif