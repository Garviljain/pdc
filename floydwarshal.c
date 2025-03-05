#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define INF 1000000 // A large value representing infinity
#define V 4         // Number of vertices

void floydWarshall(int dist[V][V]) {
    #pragma omp parallel
    {
        for (int k = 0; k < V; k++) {
            #pragma omp single
            {
                for (int i = 0; i < V; i++) {
                    for (int j = 0; j < V; j++) {
                        #pragma omp task firstprivate(i, j, k) shared(dist)
                        {
                            if (dist[i][k] != INF && dist[k][j] != INF) {
                                int new_dist = dist[i][k] + dist[k][j];
                                if (new_dist < dist[i][j]) {
                                    dist[i][j] = new_dist;
                                }
                            }
                        }
                    }
                }
                #pragma omp taskwait // Ensure all updates are completed before proceeding
            }
        }
    }
}

void printMatrix(int dist[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("INF ");
            else
                printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int graph[V][V] = {
        {0, 3, INF, 7},
        {8, 0, 2, INF},
        {5, INF, 0, 1},
        {2, INF, INF, 0}
    };

    floydWarshall(graph);

    printf("Shortest distances between every pair of vertices:\n");
    printMatrix(graph);

    return 0;
}