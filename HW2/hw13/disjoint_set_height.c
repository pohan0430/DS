#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef int bool;

typedef struct set{
	int height;
}set;

void height_union (set *, int, int);
int find_parent (set *, int);
int collapsingFind (set *, int);
bool same (set *, int , int);

int main () {
	int round, tree, operationNum, set1, set2;
	char *instruction = malloc (sizeof(char) * 7);
	scanf("%d", &round);

	for (int i = 0; i < round; i++) {
		scanf("%d %d", &tree, &operationNum);
		set *s = malloc (sizeof(set) * tree);
		for (int j = 0; j < tree; j++) {
			s[j].height = -1;
		}
		for (int j = 0; j < operationNum; j++) {
			scanf("%s", instruction);

			if (!strcmp("union", instruction)) {
				scanf("%d %d", &set1, &set2);
				height_union(s, set1, set2);
			}
			else if (!strcmp("find", instruction)) {
				scanf("%d", &set1);
				printf("%d\n", collapsingFind(s, set1));
			}
			else {
				scanf("%d %d", &set1, &set2);
				if (same(s, set1, set2)) {
					printf("true\n");
				}
				else {
					printf("false\n");
				}
			}
		}
	}

	return 0;
}

void height_union (set *s, int set1, int set2) {
	set1 = find_parent(s, set1);
	set2 = find_parent(s, set2);
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

int collapsingFind (set *s, int set1) {
	int parent = set1, i = 0; // i is path position
	int *path = malloc (sizeof(int));
	while (parent >= 0) {
		path[i++] = parent;
		parent = s[parent].height;
		path = realloc (path, sizeof(int) * (i + 1));
	}
	for (int j = 0; j < i-1; j++) {
		s[path[j]].height = path[i-1];
	}
	parent = path[i-1];
	free(path);
	return parent;
}

int find_parent (set *s, int set1) {
	int parent = set1;
	while (s[parent].height >= 0) {
		parent = s[parent].height;
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
