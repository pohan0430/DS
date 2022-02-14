#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

typedef int bool;

typedef struct Queue {
	int *data;
	int front, rear, size;
}Queue;

Queue* create (int size) {
	Queue *q = malloc (sizeof(Queue));
	q->data = malloc (sizeof(int) * size);
	q->size = size;
	q->front = -1;
	q->rear = -1;
	return q;
}

bool Empty (Queue q) {
	return q.front == q.rear;
}

bool push (Queue *q, int data) {
	if (q->rear == q->size-1) {
		return false;
	}
	else {
		q->data[++q->rear] = data;
		return true;
	}
}

int pop (Queue *q) {
	if (Empty(*q)) {
		return -1;
	}
	else {
		return q->data[++q->front];
	}
}

void bfs (int **, int *, int, int);


int main () {
	int dataSet, vertices, start;
	scanf("%d", &dataSet);

	for (int i = 0; i < dataSet; i++) {
		scanf("%d %d", &vertices, &start);
		int **graph = malloc (sizeof(int *) * vertices);
		int *visit = malloc (sizeof(int) * vertices);
		for (int j = 0; j < vertices; j++) {
			*(graph+j) = malloc (sizeof(int) * vertices);
			visit[j] = 0;
			for (int k = 0; k < vertices; k++) {
				scanf("%d", &graph[j][k]);
			}
		}

		bfs(graph, visit, start, vertices);
		for (int i = 0; i < vertices; i++) {
			if (!visit[i]) {
				bfs(graph, visit, i, vertices);
			}
		}
		printf("\n");
		free(graph);
		free(visit);
	}

	return 0;
}

void bfs (int **graph, int *visit, int start, int vertices) {
	Queue *q = create(vertices);
	visit[start] = 1;
	push(q, start);
	
	while (!Empty(*q)) {
		start = pop(q);
		printf("%d ", start);
		for (int i = 0; i < vertices; i++) {
			if (graph[start][i] && !visit[i]) {
				visit[i] = 1;
				push(q, i);
			}
		}
	}
	free(q);
}
