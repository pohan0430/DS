#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int vertex, cost; // adjacency node
	struct node *next;
}node;

void dfs (node **, int *, int *, int); 

int main () {
	int vertices, start, u, v, cost;
	scanf("%d", &vertices);

	int *visit = malloc (sizeof(int) * (vertices+1));
	int *distance = malloc (sizeof(int) * (vertices+1));
	node **adjaList = malloc (sizeof(node *) * (vertices+1));

	for (int j = 1; j <= vertices; j++) {
		visit[j] = distance[j] = 0;
		adjaList[j] = NULL;
	}

	for (int i = 0; i < vertices-1; i++) {
		node *new1 = malloc (sizeof(node));
		node *new2 = malloc (sizeof(node));
		scanf("%d %d %d", &u, &v, &cost);
		new1->cost = new2->cost = cost;
		new1->vertex = v;
		new2->vertex = u;
		if (adjaList[u] == NULL) {
			adjaList[u] = new1;
			new1->next = NULL;
		}
		else {
			new1->next = adjaList[u];
			adjaList[u] = new1;
		}
		if (adjaList[v] == NULL) {
			adjaList[v] = new2;
			new2->next = NULL;
		}
		else {
			new2->next = adjaList[v];
			adjaList[v] = new2;
		}
	}	

	scanf("%d", &start);

	visit[start] = 1;
	dfs(adjaList, visit, distance, start);
	
	for (int i = 1; i <= vertices; i++) {
		printf("%d %d\n", i, distance[i]);
	}

	free(distance);
	free(visit);
	free(adjaList);

	return 0;
}

void dfs (node **adjaList, int *visit, int *distance, int start) {
	node *p = adjaList[start];
	while (p != NULL) {
		if (!visit[p->vertex]) {
			visit[p->vertex] = 1;
			distance[p->vertex] += p->cost;
			distance[p->vertex] += distance[start];
			dfs(adjaList, visit, distance, p->vertex);
		}
		p = p->next;
	}
}
