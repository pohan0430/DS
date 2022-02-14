#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef int bool;

void create (int, int *);
void adjust_down (int *, int, int);
void adjust_up (int *, int);
int delete (int *, int *);

int main () {
	int flag, insertNum, createheap = false, HeapLength; // create min heap length
	char *operations = malloc (sizeof(char) * 10);
	char c;
	int *MinHeap = malloc (sizeof(int));

	printf("---------------Welcome----------------\n");
	printf("Press h for HELP.\n");
	do {
		printf("\n>>> ");
		scanf("%s", operations);
		if (!strcmp("Create", operations)) {
			if (createheap) {
				do {
					scanf("%c", &c);
				}while(c != '\n');
				printf("\nYou have already create a heap,\n");
			}
			else {
				scanf("%d %c", &HeapLength, &c);
				MinHeap = realloc (MinHeap, sizeof(int) * HeapLength+1);
				MinHeap[0] = 0; // meanless 
				for (int i = 1; i < HeapLength+1; i++) {
					scanf("%d", MinHeap+i);
				}
				scanf("%c", &c);
				create(HeapLength, MinHeap);
				createheap = true;
				printf("\nCreate success\n");
			}
		}
		else if (!strcmp("Remove", operations)) {
			if (!createheap) {
				do {
					scanf("%c", &c);
				}while(c != '\n');
				printf("\nYou should create heap first.\n");
			}
			else if (HeapLength == 0) {
				printf("The heap is empty now, you should insert some elements first.\n");
			}
			else {
				printf("\nRemove %d\n", delete(MinHeap, &HeapLength));
			}
		}
		else if (!strcmp("Change", operations)) {
			if (!createheap) {
				do {
					scanf("%c", &c);
				}while(c != '\n');
				printf("\nYou should create heap first.\n");
			}
			else {
				int num1, num2, pos = -1;
				scanf("%d %d", &num1, &num2);
				for (int i = 1; i < HeapLength+1; i++) {
					if (MinHeap[i] == num1) {
						pos = i;
					}
				}
				if (pos == -1) {
					printf("\nThere are no value with %d.\n", num1);
				}	
				else {
					MinHeap[pos] = num2;
					if (num2 > MinHeap[pos/2]) {
						adjust_down(MinHeap, pos, HeapLength);
					}	
					else {
						adjust_up(MinHeap, pos);
					}
					printf("\nChange success.\n");
				}
			}
		}
		else if (!strcmp("Insert", operations)) { 
			if (!createheap) {
				do {
					scanf("%c", &c);
				}while(c != '\n');
				printf("\nYou should create heap first.\n");
			}
			else {
				scanf("%d", &insertNum);
				HeapLength++;
				MinHeap = realloc (MinHeap, sizeof(int) * (HeapLength+1));
				MinHeap[HeapLength] = insertNum;
				adjust_up(MinHeap, HeapLength); 
				printf("\nInsert success.\n");
			}
		}
		else if (!strcmp("Sort", operations)) {
			if (!createheap) {
				do {
					scanf("%c", &c);
				}while(c != '\n');
				printf("\nYou should create heap first.\n");
			}
			else {
				printf("\n");
				while (HeapLength > 0) {
					printf("%d ", delete(MinHeap, &HeapLength));
				}
				printf("\n");
				free(MinHeap);
				createheap = false;
			}
		}
		else if (!strcmp("h", operations)) {
			printf("\n--------------------------------------------------------------------------------------------\n");
			printf("Commands\n");
			printf("1. Create length [element1 element2 ... ]: Create min heap length [number1 number2 ...]\n");
			printf("2. Remove                                : Remove the key with the lowest value\n");
			printf("3. Change value new_value                : Change the priority of an arbitrary element\n");
			printf("4. Insert value                          : Insert an element into heap\n");
			printf("5. h                                     : HELP\n");
			printf("6. Sort                                  : Sort the value and the heap will be destroy\n");
			printf("7. Quit                                  : Exit\n");
			printf("--------------------------------------------------------------------------------------------\n");
		}
		else if (!strcmp("Quit", operations)) {
			break;
		}
		else {
			do {
				scanf("%c", &c);
			}while(c != '\n');
			printf("\nInvalid commands, press h for HELP.\n");
		}
	}while(true);

	return 0;
}

void create (int n, int *array) {
	for (int i = n / 2; i > 0; i--) {
		adjust_down(array, i, n);
	}
}

void adjust_down (int *array, int i, int n) {
	int j = 2 * i, r = array[i];
	while (j <= n) {
		if (j < n) {
			if (array[j] > array[j+1]) {
				j++;
			}
		}
		if (r <= array[j]) {
			break;
		}
		else {
			array[j/2] = array[j];
			j *= 2;
		}
	}
	array[j/2] = r;
}

void adjust_up (int *array, int i) {
	int j = i / 2, r = array[i], k = i % 2; // k represent left or right
	while (j >= 1) {
		if (r >= array[j]) {
			break;
		}
		else {
			if (k) {
				array[j*2+1] = array[j];
				k = j % 2;
				j /= 2;
			}
			else {
				array[j*2] = array[j];
				k = j % 2;
				j /= 2;
			}
		}
	}
	if (k) {
		array[j*2+1] = r;
	}
	else {
		array[j*2] = r;
	}
}

int delete (int *array, int *n) {
	int min = array[1];
	array[1] = array[*n];
	*n -= 1;
	array = realloc (array, ((*n)+1) *sizeof(int));
	adjust_down(array, 1, *n);
	return min;
}
