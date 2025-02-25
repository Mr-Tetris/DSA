#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_EDGE -1
#define MAX_SIZE 1500

struct Edge {
    int vertex;
    int weight;
};

struct PriorityQueue {
    struct Edge elements[MAX_SIZE];
    int size;
};

void initGraph(int **graph, int n) {
    for (int i = 0; i < n; i++) {
        memset(graph[i], NO_EDGE, n * sizeof(int));
    }
}

void push(struct PriorityQueue *pq, struct Edge edge) {
    if (pq->size >= MAX_SIZE) {
        printf("Priority queue overflow\n");
        return;
    }
    pq->elements[pq->size] = edge;
    int current = pq->size++;
    while (current > 0 && pq->elements[current].weight < pq->elements[(current - 1) / 2].weight) {
        struct Edge tmp = pq->elements[(current - 1) / 2];
        pq->elements[(current - 1) / 2] = pq->elements[current];
        pq->elements[current] = tmp;
        current = (current - 1) / 2;
    }
}

struct Edge pop(struct PriorityQueue *pq) {
    struct Edge min = pq->elements[0];
    pq->elements[0] = pq->elements[--pq->size];
    int current = 0;
    while (2 * current + 1 < pq->size) {
        int child = 2 * current + 1;
        if (child + 1 < pq->size && pq->elements[child + 1].weight < pq->elements[child].weight) {
            child++;
        }
        if (pq->elements[current].weight <= pq->elements[child].weight) {
            break;
        }
        struct Edge tmp = pq->elements[child];
        pq->elements[child] = pq->elements[current];
        pq->elements[current] = tmp;
        current = child;
    }
    return min;
}

int isEmpty(struct PriorityQueue *pq) {
    return pq->size == 0;
}

void dijkstra(int **graph, int n, int src, int target, int first) {
    struct PriorityQueue pq;
    pq.size = 0;

    int *dist = malloc(n * sizeof(int));
    int *prev = malloc(n * sizeof(int));
    int *visited = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = NO_EDGE;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[src] = 0;
    push(&pq, (struct Edge){src, 0});

    while (!isEmpty(&pq)) {
        struct Edge e = pop(&pq);
        int u = e.vertex;
        if (visited[u]) continue;
        visited[u] = 1;
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != NO_EDGE && !visited[v]) {
                int newDist = dist[u] + graph[u][v];
                if (dist[v] == NO_EDGE || dist[v] > newDist) {
                    dist[v] = newDist;
                    prev[v] = u;
                    push(&pq, (struct Edge){v, newDist});
                }
            }
        }
    }

    if (dist[target] == NO_EDGE) {
        printf(first ? "search %d %d failed" : "\nsearch %d %d failed", src, target);
    } else {
        printf(first ? "%d: [" : "\n%d: [", dist[target]);
        int *path = malloc(n * sizeof(int));
        int path_index = 0;
        for (int at = target; at != -1; at = prev[at]) {
            path[path_index++] = at;
        }
        for (int i = path_index - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(", ");
        }
        printf("]");
        free(path);
    }

    free(dist);
    free(prev);
    free(visited);
}

int main() {
    int n, m, u, v, w;
    char operation;
    int first = 1;

    scanf("%d %d", &n, &m);

    int **graph = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = malloc(n * sizeof(int));
    }

    initGraph(graph, n);

    for (int i = 0; i < m; i++) {
        scanf(" (%d, %d, %d)", &u, &v, &w);
        if (u < 0 || u >= n || v < 0 || v >= n || u == v || w < 0) {
            printf("insert %d %d failed\n", u, v);
        }
        if (graph[u][v] != NO_EDGE || graph[v][u] != NO_EDGE) {
            printf("insert %d %d failed\n", u, v);
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }

    while (scanf(" %c", &operation) != EOF) {
        switch (operation) {
            case 'i':
                scanf(" %d %d %d", &u, &v, &w);
                if (u < 0 || u >= n || v < 0 || v >= n || u == v || w < 0 || graph[u][v] != NO_EDGE || graph[v][u] != NO_EDGE) {
                    if (first == 1){
                        printf("insert %d %d failed", u, v);
                        first = 0;
                    } else{
                        printf("\ninsert %d %d failed", u, v);
                    }
                } else {
                    graph[u][v] = w;
                    graph[v][u] = w;
                }
                break;
            case 'd':
                scanf(" %d %d", &u, &v);
                if (u < 0 || u >= n || v < 0 || v >= n || u == v || graph[u][v] == NO_EDGE || graph[v][u] == NO_EDGE) {
                    if (first == 1){
                        printf("delete %d %d failed", u, v);
                        first = 0;
                    } else{
                        printf("\ndelete %d %d failed", u, v);
                    }
                } else {
                    graph[u][v] = NO_EDGE;
                    graph[v][u] = NO_EDGE;
                }
                break;
            case 'u':
                scanf(" %d %d %d", &u, &v, &w);
                if (u < 0 || u >= n || v < 0 || v >= n || u == v) {
                    if (first == 1){
                        printf("update %d %d failed", u, v);
                        first = 0;
                    } else{
                        printf("\nupdate %d %d failed", u, v);
                    }
                } else if (graph[u][v] == NO_EDGE || graph[v][u] == NO_EDGE) {
                    if (first == 1){
                        printf("update %d %d failed", u, v);
                        first = 0;
                    } else{
                        printf("\nupdate %d %d failed", u, v);
                    }
                } else {
                    if (graph[u][v] + w < 0 || graph[v][u] + w < 0) { // Zabezpečí, že nová váha nie je záporná
                        if (first == 1) {
                            printf("update %d %d failed", u, v);
                            first = 0;
                        } else {
                            printf("\nupdate %d %d failed", u, v);
                        }
                    } else {
                        graph[u][v] += w;
                        graph[v][u] += w;
                    }
                }
                break;
            case 's':
                scanf(" %d %d", &u, &v);
            if (u < 0 || u >= n || v < 0 || v >= n) {
                if (first == 1){
                    printf("search %d %d failed", u, v);
                    first = 0;
                } else{
                    printf("\nsearch %d %d failed", u, v);
                }
            } else {
                dijkstra(graph, n, u, v, first);
                first = 0;
            }
            break;
        }

        for (int i = 0; i < n; i++) {
            free(graph[i]);
        }
        free(graph);

        return 0;
    }
}