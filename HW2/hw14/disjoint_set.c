#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define true 1
#define false 0

typedef int bool;

typedef struct set{
	int weight;
}set;

void height_union (set *, int, int);
void weight_union (set *, int, int);
int collapsingFind (set *, int);
int find_parent (set *, int);
bool same (set *, int , int);

int main () {
	clock_t start, end;
       	double totalTime[2] = {0};
	int round = 10, tree = 10000, operationNum = 1000000;
	char *instruction = malloc (sizeof(char) * 7);
	srand(time(NULL));
	int *op =  malloc (sizeof(int) * operationNum);
       	int *element = malloc (sizeof(int) * (2 * operationNum) + 2);
	for (int i = 0, j = 0; j < operationNum; j++) {
		op[j] = rand() % 3;
		if (op[j] == 1) {
			element[i++] = rand() % tree;
		}
		else {
			element[i++] = rand() % tree;
			element[i++] = rand() % tree;
		}
	}
	
	set *s = malloc (sizeof(set) * tree);
	for (int j = 0; j < tree; j++) {
			s[j].weight = -1;
	}

	// Weight Union
	start = clock();
	for (int i = 0; i < round; i++) {
		for (int j = 0, k = 0; j < operationNum; j++) {
			if (op[j] == 0) {
				weight_union(s, element[k++], element[k++]);
			}
			else if (op[j] == 1) {
				collapsingFind(s, element[k++]);
			}
			else {
				same(s, element[k++], element[k++]);
			}
		}
	}
	end = clock();
	totalTime[0] += (double) (end-start)/CLOCKS_PER_SEC;

	for (int j = 0; j < tree; j++) {
		s[j].weight = -1;
	}

	// Height Union
	start = clock();
	for (int i = 0; i < round; i++) {
		for (int j = 0, k = 0; j < operationNum; j++) {
			if (op[j] == 0) {
				height_union(s, element[k++], element[k++]);
			}
			else if (op[j] == 1) {
				collapsingFind(s, element[k++]);
			}
			else {
				same(s, element[k++], element[k++]);
			}
		}
	}
	end = clock();
	totalTime[1] += (double) (end-start)/CLOCKS_PER_SEC;
	free(s);
	
	printf("round: %d\n", round);
	printf("Set size: %d\n", tree);
	printf("Number of operations need to do: %d\n", operationNum); 
	printf("Weight Union: %lf (second)\n", totalTime[0]); 
	printf("Height Union: %lf (second)\n", totalTime[1]); 


	return 0;
}

void weight_union (set *s, int set1, int set2) {
	set1 = find_parent(s, set1);
	set2 = find_parent(s, set2);
	if (set1 != set2) {
		if (s[set1].weight <= s[set2].weight) {
			s[set1].weight = s[set1].weight + s[set2].weight;
			s[set2].weight = set1;
		}
		else {
			s[set2].weight = s[set1].weight + s[set2].weight;
			s[set1].weight = set2;
		}
	}
}

void height_union (set *s, int set1, int set2) {
	set1 = find_parent(s, set1);
	set2 = find_parent(s, set2);
	if (set1 != set2) {
		if (s[set1].weight == s[set2].weight) {
			s[set1].weight--;
			s[set2].weight = set1;
		}
		else if (s[set1].weight < s[set2].weight){
			s[set2].weight = set1;
		}
		else {
			s[set1].weight = set2;
		}
	}
}

int collapsingFind (set *s, int set1) {
	int parent = set1, i = 0; // i is path position
	int *path = malloc (sizeof(int));
	while (parent >= 0) {
		path[i++] = parent;
		parent = s[parent].weight;
		path = realloc (path, sizeof(int) * (i + 1));
	}
	for (int j = 0; j < i-1; j++) {
		s[path[j]].weight = path[i-1];
	}
	parent = path[i-1];
	free(path);
	return parent;
}

int find_parent (set *s, int set1) {
	int parent = set1;
	while (s[parent].weight >= 0) {
		parent = s[parent].weight;
	}
	return parent;
}

bool same (set *s, int set1, int set2) {
	if (find_parent(s, set1) == find_parent(s, set2)) {
		return true;
	}
	else {
		return false;
	}
}
