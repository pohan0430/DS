#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void adjust_up (int *, int);
void min_adjust_down (int *);
void max_adjust_down (int *);
void insert (int *, int);
void deleteMin (int *);
void deleteMax (int *);
void show (int *);

int size = 1;
int length = 1000;

int main () {
	char op[7];
	int element;
	int *SMMH = (int *)malloc (sizeof(int) * length);
	
	while (1) {
		scanf("%s", op);

		if (!strcmp(op, "insert")) {
			scanf("%d", &element);
			insert(SMMH, element);
		}
		else if (!strcmp(op, "delete")) {
			scanf("%s", op);
			if (!strcmp(op, "min")) {
				deleteMin(SMMH);
			}
			else {
				deleteMax(SMMH);
			}
		}
		else if (!strcmp(op, "show")) {
			show(SMMH);
		}
		else {
			break;
		}
	}

	return 0;
}

void min_adjust_down (int *SMMH) {
	int r = SMMH[2], parent = 2, left = parent * 2, right = (parent + 1) * 2, comp, pos;

	while (left <= size) {
		comp = SMMH[left];
		pos = left;

		if (right <= size && SMMH[right] < SMMH[left]) {
			comp = SMMH[right];
			pos = right;
		}

		if (comp < r) {
			SMMH[parent] = comp;
			parent = pos;
			left = parent * 2;
			right = (parent + 1) * 2;
		}
		else {
			break;
		}
	}
		
	if (parent % 2 && SMMH[parent-1] > r) {
		SMMH[parent] = SMMH[parent-1];
		SMMH[parent-1] = r;
	}
	else if (parent % 2 == 0 && SMMH[parent+1] < r) {
		SMMH[parent] = SMMH[parent+1];
		SMMH[parent+1] = r;
	}
	else {
		SMMH[parent] = r;
	}
}

void max_adjust_down (int *SMMH) {
	int r = SMMH[3], parent = 3, left = (parent - 1) * 2 + 1, right = parent * 2 + 1, comp, pos;

	while (left <= size) {
		comp = SMMH[left];
		pos = left;

		if (right <= size && SMMH[right] > SMMH[left]) {
			comp = SMMH[right];
			pos = right;
		}

		if (comp > r) {
			SMMH[parent] = comp;
			parent = pos;
			left = (parent - 1) * 2 + 1;
			right = parent * 2 + 1;
		}
		else {
			break;
		}
	}

	if (parent % 2 && SMMH[parent-1] > r) {
		SMMH[parent] = SMMH[parent-1];
		SMMH[parent-1] = r;
	}
	else if (parent % 2 == 0 && SMMH[parent+1] < r) {
		SMMH[parent] = SMMH[parent+1];
		SMMH[parent+1] = r;
	}
	else {
		SMMH[parent] = r;
	}
}

void adjust_up (int *SMMH, int n) {
	int r = SMMH[n], now = n, min, max;

	if (now % 4 >= 2) {
		min = now / 2 - 1;
		max = now / 2;
	}
	else {
		min = now / 2;
		max = now / 2 + 1;
	}

	while (min >= 2) {
		if (r < SMMH[min]) {
			SMMH[now] = SMMH[min];
			now = min;
			if (now % 4 >= 2) {
				min = now / 2 - 1;
				max = now / 2;
			}
			else {
				min = now / 2;
				max = now / 2 + 1;
			}
		}
		else if (r > SMMH[max]) {
			SMMH[now] = SMMH[max];
			now = max;
			if (now % 4 >= 2) {
				min = now / 2 - 1;
				max = now / 2;
			}
			else {
				min = now / 2;
				max = now / 2 + 1;
			}
		}
		else {
			break;
		}
	}

	SMMH[now] = r;
}

void insert (int *SMMH, int element) {
	size++;
	if (size > length-1) {
		length += 1000;
		int *p = (int *) realloc (SMMH, sizeof(int) * (length));
		if (p == NULL) {
			printf("error\n");
		}
		else {
			SMMH = p;
		}
	}
	
	if (size % 2 && element < SMMH[size-1]) {
		SMMH[size] = SMMH[size-1];
		SMMH[size-1] = element;
		adjust_up(SMMH, size-1);
	}
	else {
		SMMH[size] = element;		
		adjust_up(SMMH, size);
	}
}

void deleteMin (int *SMMH) {
	SMMH[2] = SMMH[size--];
	min_adjust_down(SMMH);
}

void deleteMax (int *SMMH) {
	SMMH[3] = SMMH[size--];
	max_adjust_down(SMMH);
}

void show (int *SMMH) {
	printf("NULL\n");
	
	int i = 2;
	while (i <= size) {
		for (int j = i; j < i*2 && j <= size; j++) {
			printf("%d ", SMMH[j]);
		}
		printf("\n");
		i *= 2;
	}
}


