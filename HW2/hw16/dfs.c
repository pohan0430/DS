#include <stdio.h>
#include <stdlib.h>

void dfs (int **, int *, int, int); 

int main () {
	int dataSet, vertices, start;
	scanf("%d", &dataSet);

	for (int i = 0; i < dataSet; i++) {
		scanf("%d %d", &vertices, &start);
		int **graph =  malloc (sizeof(int *) * vertices);
		int *visit = malloc (sizeof(int) * vertices);
		for (int j = 0; j < vertices; j++) {
			*(graph+j) = malloc (sizeof(int) * vertices);
			visit[j] = 0;
			for (int k = 0; k < vertices; k++) {
				scanf("%d", &graph[j][k]);
			}
		}
		visit[start] = 1;
		printf("%d ", start); 
		dfs(graph, visit, vertices, start);
		for (int i = 0; i < vertices; i++) {
			if (!visit[i]) {
				visit[i] = 1;
				printf("%d ", i); 
				dfs(graph, visit, vertices, start);
			}
		}
		printf("\n");
		free(graph);
		free(visit);
	}

	return 0;
}

void dfs (int **graph, int *visit, int vertices, int start) {
	for (int i = 0; i < vertices; i++) {
		if (graph[start][i]) {
			if (!visit[i]) {
				visit[i] = 1;
				printf("%d ", i);
				dfs(graph, visit, vertices, i);
			}
		}
	}
}

			
