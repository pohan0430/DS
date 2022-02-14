#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int row;
	int column;
	int value;
}node;

int main () {
	int R, C, row, column, keys, value, i;
	scanf("%d %d %d", &R, &C, &keys);
	int count[C], start[C];
       	for (i = 0; i < C; i++) {
		count[i] = 0;
		start[i] = 0;
	}	
	node *matrix = (node *) malloc (sizeof(node) * keys);
	node *output = (node *) malloc (sizeof(node) * keys);
	for (i = 0; i < keys; i++) {
		scanf("%d %d %d", &row, &column, &value);
		count[column]++;
		matrix[i].row = row;
		matrix[i].column = column;
		matrix[i].value = value;
	}
	for (i = 1; i < C; i++) {
		start[i] = count[i-1] + start[i-1];
	}
	for (i = 0; i < keys; i++) {
		output[start[matrix[i].column]].row = matrix[i].column;
		output[start[matrix[i].column]].column = matrix[i].row;
		output[start[matrix[i].column]].value = matrix[i].value;
		start[matrix[i].column]++;
	}

	printf("%d %d %d\n", R, C, keys); 
	for (int i = 0; i < keys; i++) {
		printf("%d %d %d\n", output[i].row, output[i].column, output[i].value);
	}

	return 0;
}

