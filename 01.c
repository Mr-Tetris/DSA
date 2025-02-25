#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1500
#define NO_EDGE -1
#define INFINITY 999999999

typedef struct {
    int vertex;
    int priority;
} PriorityQueueNode;

typedef struct {
    PriorityQueueNode* nodes;
    int size;
} PriorityQueue;


void initPriorityQueue(PriorityQueue *pq, int capacity) {
    pq->nodes = (PriorityQueueNode *)malloc(capacity * sizeof(PriorityQueueNode));
    pq->size = 0;
}

void swap(PriorityQueueNode *a, PriorityQueueNode *b) {
    PriorityQueueNode temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleUp(PriorityQueue *pq, int index) {
    while (index > 0 && pq->nodes[index].priority < pq->nodes[(index - 1) / 2].priority) {
        swap(&pq->nodes[index], &pq->nodes[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void insert(PriorityQueue *pq, int vertex, int priority) {

    pq->nodes[pq->size].vertex = vertex;
    pq->nodes[pq->size].priority = priority;
    bubbleUp(pq, pq->size);
    pq->size++;
}

void bubbleDown(PriorityQueue *pq, int index) {
    int smallest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < pq->size && pq->nodes[leftChild].priority < pq->nodes[smallest].priority) {
        smallest = leftChild;
    }
    if (rightChild < pq->size && pq->nodes[rightChild].priority < pq->nodes[smallest].priority) {
        smallest = rightChild;
    }
    if (smallest != index) {
        swap(&pq->nodes[index], &pq->nodes[smallest]);
        bubbleDown(pq, smallest);
    }
}

PriorityQueueNode extractMin(PriorityQueue *pq) {

    PriorityQueueNode min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;
    bubbleDown(pq, 0);
    return min;
}

int isEmpty(PriorityQueue *pq) {
    return pq->size == 0;
}






typedef struct Edge {
    int src;
    int dest;
    int weight;
    struct Edge *next;
} Edge;

typedef struct {
    Edge **vertices;
    int numVertices;
} Graph;

void initGraph(Graph *g, int n) {
    g->numVertices = n;
    g->vertices = malloc(sizeof(Edge *) * g->numVertices);

    for(int i = 0; i < g->numVertices; i++) {
        g->vertices[i] = NULL;
    }
}

int addEdge(Graph *g, int u, int v, int w) {
    if (u < 0 || u >= g->numVertices || v < 0 || v >= g->numVertices || u == v || w < 0) {
        return -1;
    }
    Edge* current = g->vertices[u];
    while (current) {
        if (current->dest == v) {
            return -1;
        }
        current = current->next;
    }

    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->src = u;
    newEdge->dest = v;
    newEdge->weight = w;
    newEdge->next = g->vertices[u];
    g->vertices[u] = newEdge;

    return 0;
}

int deleteEdge(Graph *g, int u, int v) {
    Edge **ptr = &(g->vertices[u]);
    while (*ptr) {
        if ((*ptr)->src == u && (*ptr)->dest == v) {
            Edge *temp = *ptr;
            *ptr = (*ptr)->next;
            free(temp);
            return 0;
        }
        ptr = &((*ptr)->next);
    }

    return -1;
}

int updateEdge(Graph *g, int u, int v, int w) {
    Edge *ptr = g->vertices[u];
    while (ptr) {
        if (ptr->src == u && ptr->dest == v) {
            if (ptr->weight + w < 0) {
                return -1;
            }
            ptr->weight += w;
            return 0;
        }
        ptr = ptr->next;
    }
    return -1;
}

void dijkstra(Graph *g, int src, int dest, int first) {
    int *dist = malloc(g->numVertices * sizeof(int));
    int *visited = malloc(g->numVertices * sizeof(int));
    int *prev = malloc(g->numVertices * sizeof(int));
    for (int i = 0; i < g->numVertices; i++) {
        dist[i] = INFINITY;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[src] = 0;

    PriorityQueue pq;
    initPriorityQueue(&pq, g->numVertices * 2);

    insert(&pq, src, 0);

    while (!isEmpty(&pq)) {
        PriorityQueueNode node = extractMin(&pq);
        int minIndex = node.vertex;
        visited[minIndex] = 1;

        Edge *e = g->vertices[minIndex];
        while (e) {
            int v = e->dest;
            if (visited[v] != 1 && dist[minIndex] + e->weight < dist[v]) {
                dist[v] = dist[minIndex] + e->weight;
                prev[v] = minIndex;
                insert(&pq, v, dist[v]);
            }
            e = e->next;
        }
    }

    if (dist[dest] == INFINITY) {
        printf(first ? "search failed" : "\nsearch failed");
    } else {
        printf(first ? "%d: [" : "\n%d: [", dist[dest]);
        int u = dest;
        int path[MAX_SIZE], pathSize = 0;
        while (u != -1) {
            path[pathSize++] = u;
            u = prev[u];
        }
        for (int i = pathSize - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(", ");
        }
        printf("]");
    }

    free(dist);
    free(visited);
    free(prev);
    free(pq.nodes);
}

int main() {
    int n, m, u, v, w;
    char operation;
    int first = 1;

    scanf("%d %d", &n, &m);
    Graph g;
    initGraph(&g, n);

    for (int i = 0; i < m; i++) {
        scanf(" (%d, %d, %d)", &u, &v, &w);
        addEdge(&g, u, v, w);
        addEdge(&g, v, u, w);
    }

    while (scanf(" %c", &operation) != EOF) {
        switch (operation) {
            case 'i':
                scanf(" %d %d %d", &u, &v, &w);
                addEdge(&g, u, v, w);
                if(addEdge(&g, v, u, w) == -1) {
                    printf(first ? "insert %d %d failed" : "\ninsert %d %d failed", u, v);
                    first = 0;
                }
                break;
            case 'd':
                scanf(" %d %d", &u, &v);
                deleteEdge(&g, u, v);
                if(deleteEdge(&g, v, u) == -1) {
                    printf(first ? "delete %d %d failed" : "\ndelete %d %d failed", u, v);
                    first = 0;
                }
                break;
            case 'u':
                scanf(" %d %d %d", &u, &v, &w);
                updateEdge(&g, u, v, w);
                if(updateEdge(&g, v, u, w) == -1) {
                    printf(first ? "update %d %d failed" : "\nupdate %d %d failed", u, v);
                    first = 0;
                }
                break;
            case 's':
                scanf(" %d %d", &u, &v);
                dijkstra(&g, u, v, first);
                first = 0;
                break;
        }
    }

    for (int i = 0; i < g.numVertices; i++) {
        Edge *e = g.vertices[i];
        while (e) {
            Edge *next = e->next;
            free(e);
            e = next;
        }
    }

    return 0;
}