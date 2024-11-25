#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct Node {
    int vertex;
    int fluxo;
    int capacidade;
    struct Node* next; 
} Node;

// Função para adicionar um vértice à lista de adjacência
void criando_lista_fluxo(Node** adjList, int a, int b, int fluxo, int capacidade) {
    Node* no = (Node*)malloc(sizeof(Node));
    no->vertex = b;
    no->fluxo = fluxo;
    no->capacidade = capacidade;
    no->next = adjList[a];
    adjList[a] = no;
}

// Função para criar a lista de adjacência
Node** lista(FILE* grafo, int* n) {
    if (fscanf(grafo, "%d", n) != 1) {
        printf("Erro ao ler o número de vértices\n");
        return NULL;
    }    
    Node** lista = (Node**)malloc(*n * sizeof(Node*));
    for (int i = 0; i < *n; i++) {
        lista[i] = NULL;
    }

    int origem, destino, capacidade;
    while (fscanf(grafo, "%d %d %d", &origem, &destino, &capacidade) == 3) {
        criando_lista_fluxo(lista, origem - 1, destino - 1, 0, capacidade);
    }
    return lista;
}

// Função para criar o grafo residual
Node** GrafoResidual(Node** grafog, int n) {
    Node** residual = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) {
        residual[i] = NULL;
    }

    for (int i = 0; i < n; i++) {
        Node* current = grafog[i];
        while (current != NULL) {
            if (current->capacidade > 0) {
                criando_lista_fluxo(residual, i, current->vertex, 0, current->capacidade - current->fluxo);
            }
            if (current->fluxo > 0) {
                criando_lista_fluxo(residual, current->vertex, i, 0, current->fluxo);
            }
            current = current->next;
        }
    }

    return residual;
}

// Função para encontrar o gargalo de um caminho
int Gargalo(Node** adjList, int* caminho, int tamanhoCaminho) {
    int menorcap = INT_MAX;
    for (int i = 0; i < tamanhoCaminho - 1; i++) {
        int origem = caminho[i];
        int destino = caminho[i + 1];
        Node* atual = adjList[origem];
        while (atual != NULL) {
            if (atual->vertex == destino) {
                int caparesidual = atual->capacidade - atual->fluxo;
                if (caparesidual < menorcap) {
                    menorcap = caparesidual; // atualizando o gargalo
                }
                break;
            }
            atual = atual->next;
        }
    }
    return menorcap;
}

// Busca DFS para encontrar caminho aumentante
int* DFS_Caminho(Node** lista, int origem, int destino, int n, int* tamanhoCaminho) {
    int* visitados = (int*)calloc(n, sizeof(int));
    int* pai = (int*)malloc(n * sizeof(int));
    int* stack = (int*)malloc(n * sizeof(int));
    int top = -1;

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
    }

    stack[++top] = origem;
    visitados[origem] = 1;

    while (top != -1) {
        int atual = stack[top--];
        Node* ponteiro = lista[atual];

        while (ponteiro) {
            int vv = ponteiro->vertex;
            if (!visitados[vv] && ponteiro->capacidade > 0) {
                stack[++top] = vv;
                visitados[vv] = 1;
                pai[vv] = atual;
                if (vv == destino) {
                    break;
                }
            }
            ponteiro = ponteiro->next;
        }
    }

    if (pai[destino] == -1) {
        free(visitados);
        free(stack);
        free(pai);
        *tamanhoCaminho = 0;
        return NULL;
    }

    int* caminho = (int*)malloc(n * sizeof(int));
    int idx = 0;
    for (int v = destino; v != -1; v = pai[v]) { // encontrando o caminho ao contrário
        caminho[idx++] = v;
    }

    *tamanhoCaminho = idx;

    for (int i = 0; i < idx / 2; i++) { // revertendo o caminho
        int temp = caminho[i];
        caminho[i] = caminho[idx - i - 1];
        caminho[idx - i - 1] = temp;
    }

    free(visitados);
    free(stack);
    free(pai);
    return caminho;
}

// Atualiza os fluxos e capacidades após encontrar um caminho aumentante
void Augment(Node** Residual, Node** original, int* caminho, int tamanho, int gargalo) {
    for (int i = 0; i < tamanho - 1; i++) {
        int origem = caminho[i];
        int destino = caminho[i + 1];

        Node* atual = Residual[origem];
        while (atual != NULL) {
            if (atual->vertex == destino) {
                atual->capacidade -= gargalo;
                break;
            }
            atual = atual->next;
        }

        Node* origAtual = original[origem];
        while (origAtual != NULL) {
            if (origAtual->vertex == destino) {
                origAtual->fluxo += gargalo;
                break;
            }
            origAtual = origAtual->next;
        }

        atual = Residual[destino];
        while (atual != NULL) {
            if (atual->vertex == origem) {
                atual->capacidade += gargalo;
                break;
            }
            atual = atual->next;
        }
    }
}

// Algoritmo de Ford-Fulkerson
int FordFulkerson(Node** Original, Node** Residual, int source, int target, int n) {
    int fluxoMaximo = 0;

    while (1) {
        int tamanhoCaminho;
        int* caminho = DFS_Caminho(Residual, source - 1, target - 1, n, &tamanhoCaminho);

        if (caminho == NULL) { 
            break; // não há mais caminho aumentante
        }

        int gargalo = Gargalo(Residual, caminho, tamanhoCaminho);
        Augment(Residual, Original, caminho, tamanhoCaminho, gargalo);
        fluxoMaximo += gargalo;

        free(caminho);
    }

    return fluxoMaximo;
}

void imprimirFluxos(Node** adjList, int numVertices, FILE* qvertice) { //Função para imprimir o fluxo em cada aresta
    fprintf(qvertice, "Fluxos em cada aresta:\n");
    for (int i = 0; i < numVertices; i++) {
        Node* atual = adjList[i];
        while (atual != NULL) {
            fprintf(qvertice, "Aresta %d -> %d: Fluxo = %d / Capacidade = %d\n", 
                   i + 1, atual->vertex + 1, atual->fluxo, atual->capacidade);
            atual = atual->next;
        }
    }
}


// Função principal
int main(void) {
    char arquivo[10000];
    printf("Digite o arquivo txt: ");
    fgets(arquivo, sizeof(arquivo), stdin);
    arquivo[strcspn(arquivo, "\n")] = 0; // Remove a nova linha

    FILE* grafo = fopen(arquivo, "r");
    if (grafo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }
    FILE* qvertice = fopen("resultados.txt", "wt");
    
    int numVertices;
    Node** adjListOriginal = lista(grafo, &numVertices);
    fclose(grafo); 

    if (adjListOriginal == NULL) {
        printf("Erro ao criar o grafo original.\n");
        return 1;
    }

    Node** adjListResidual = GrafoResidual(adjListOriginal, numVertices);
    if (adjListResidual == NULL) {
        printf("Erro ao criar o grafo residual.\n");
        return 1;
    }
    int s, t;
    printf("Escolha um vértice para ser a Origem: ");
    scanf("%d", &s);
    printf("Escolha um vértice para ser o Destino: ");
    scanf("%d", &t);

    int fluxoMaximo = FordFulkerson(adjListOriginal, adjListResidual, s, t, numVertices);
    fprintf(qvertice, "Fluxo máximo: %d \n", fluxoMaximo);

    for (int j = 0; j < numVertices; j++) {
        Node* atual = adjListResidual[j];
        while (atual != NULL) {
            Node* temp = atual;
            atual = atual->next;
            free(temp);
        }
    }
    free(adjListResidual);

    imprimirFluxos(adjListOriginal, numVertices, qvertice);

    for (int i = 0; i < numVertices; i++) {
        Node* atual = adjListOriginal[i];
        while (atual != NULL) {
            Node* temp = atual;
            atual = atual->next;
            free(temp);
        }
    }
    free(adjListOriginal);

    fclose(qvertice);
    return 0;
}
