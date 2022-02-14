#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef int bool;

typedef struct set{
	int height;
}set;

typedef struct Edge{
	int u, v, c;
}Edge;

void height_union (set *, int, int);
int find (set *, int);
bool same (set *, int , int);

void create_minHeap (int, Edge *);
void adjust (Edge *, int, int);
Edge delete (Edge *, int *);

long long Kruskal (Edge *, set *, int, int);

int main () {
	int vertices, edges;

	scanf("%d %d", &vertices, &edges);
	Edge *edgeList = malloc (sizeof(Edge) * (edges+1)); 
	set *s1 = malloc (sizeof(set) * vertices);

	for (int i = 1; i <= edges; i++) {
		scanf("%d %d %d", &edgeList[i].u, &edgeList[i].v, &edgeList[i].c);
	}

	for (int i = 0; i < vertices; i++) {
		s1[i].height = -1;
	}

	printf("%lld\n", Kruskal(edgeList, s1, vertices, edges));	

	return 0;
}

void create_minHeap (int n, Edge *array) {
	int i = n / 2;
	for (i;i>0;i--) {
		adjust(array, i, n);
	}
}

void adjust (Edge *array, int i, int n) {
	int j = 2 * i, r = array[i].c;
	Edge temp = array[i];
	while (j <= n) {
		if (j < n) {
			if (array[j].c > array[j+1].c) {
				j++;
			}
		}
		if (r <= array[j].c) {
			break;
		}
		else {
			array[j/2] = array[j];
			j *= 2;
		}
	}
	array[j/2] = temp;
}

Edge delete (Edge *array, int *n) {
	Edge min = array[1];
	array[1] = array[*n];
	*n -= 1;
	array = realloc (array, ((*n)+1) * sizeof(Edge));
	adjust(array, 1, *n);
	return min;
}

void height_union (set *s, int set1, int set2) {
	set1 = find(s, set1);
	set2 = find(s, set2);
	if (set1 != set2) {
		if (s[set1].height == s[set2].height) {
			s[set1].height--;
			s[set2].height = set1;
		}
		else if (s[set1].height < s[set2].height){
			s[set2].height = set1;
		}
		else {
			s[set1].height = set2;
		}
	}
}

int find (set *s, int set1) {
	int parent = set1, i = 0; // i is path position
	int *path = malloc (sizeof(int));
	while (parent >= 0) {
		path[i] = parent;
		parent = s[parent].height;
		i++;
		path = realloc (path, sizeof(int) * (i + 1));
	}
	for (int j = 0; j < i-1; j++) {
		s[path[j]].height = path[i-1];
	}
	parent = path[i-1];
	free(path);
	return parent;
}

bool same (set *s, int set1, int set2) {
	if (find(s, set1) == find(s, set2)) {
		return true;
	}
	else {
		return false;
	}
}

long long Kruskal(Edge *edgeList, set *s1, int vertices, int edges) {
	long long totalCost;
	int i = 0;

	create_minHeap(edges, edgeList);

	while (i < vertices-1) {
		Edge min = delete(edgeList, &edges);
		if (find(s1, min.u) != find(s1, min.v)) {
			height_union(s1, min.u, min.v);
			totalCost += min.c;
			i++;
		}
	}

	return totalCost;
}


