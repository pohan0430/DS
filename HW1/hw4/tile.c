#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MoveDirection 8

int *tilesWalk(int, int, int, int, int*);  
void showtiles(int*, int, int, int);

int main () {
	srand(time(NULL));
	int n[5] = {0, 0, 0, 15, 39}, m[5] = {0, 0, 0, 15, 19}, total[5] = {0, 0, 0, 0, 0}, a;
	int **tiles = malloc (5 * sizeof(int *));
	char c;
	for (int i = 0; i < 3; i++) {
		scanf("%c %d %d %c", &c, n+i, m+i, &c);
		tiles[i] = tilesWalk(n[i], m[i], 0, 0, &total[i]);
	}
	
	scanf("%c %d %c", &c, &a, &c);
	scanf("%c %d %c", &c, &a, &c);
	scanf("%c", &c);

	tiles[3] = tilesWalk(15, 15, 9, 9, &total[3]);
	tiles[4] = tilesWalk(39, 19, 0, 0, &total[4]);

	for (int k = 0; k < 5; k++) {
		showtiles(tiles[k], n[k], m[k], total[k]);
	}

	return 0;
}

int *tilesWalk(int n, int m, int istart, int jstart, int *total){  
	int imove[MoveDirection] = {-1, 0, 1, 1, 1, 0, -1, -1};
	int jmove[MoveDirection] = {1, 1, 1, 0, -1, -1, -1, 0};
	int *tiles_count = malloc (n * m * sizeof(int));
	int count = 0, randnum;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			*(tiles_count+i*m+j) = 0;
		}
	}

	*(tiles_count+istart*m+jstart) = 1;
	count += 1;
	*total += 1;

	while (count < n * m) {
		randnum = rand() % 8;
		if (istart + imove[randnum] < n && istart + imove[randnum] >= 0 &&  jstart + jmove[randnum] < m && jstart + jmove[randnum] >= 0) {
			istart += imove[randnum];
			jstart += jmove[randnum];
			*(tiles_count+istart*m+jstart) += 1;
			*total += 1;
			if (*(tiles_count+istart*m+jstart) == 1) {
				count++;
			}
		}
		else {
			continue;
		}
	}
	return tiles_count;
}

void showtiles(int *tiles, int n, int m, int total) {
	printf("%d\n", total); 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%4d", *(tiles+i*m+j));
		}
		printf("\n");
	}
}
