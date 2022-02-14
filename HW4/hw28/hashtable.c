#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define HASH_SIZE 19

typedef struct {
	int key, value;
}element;

void insert (element **, element, int *, double *);
element *reset (element *, int *, double *);
int search (element *, int, int);
int delete (element *, int, int);

int main () {
	srand(time(NULL));
	int size = 1;
	double density = 0;
	element *hash = malloc (sizeof(element) * HASH_SIZE * size);
	for (int i = 0; i < HASH_SIZE * size; i++) {
		hash[i].key = hash[i].value = -1;
	}

	element e;
	for (int i = 0; i < 15; i++) {
		int randKey = rand() % 50;
		int randValue = rand() % 1000;
		if (search(hash, randKey, size) != -1) {
			continue;
		}
		e.key = randKey;
		e.value = randValue;
		insert(&hash, e, &size, &density);
		printf("insert key:%d value: %d\n", e.key, e.value);
	}

	printf("--------------------------------------------\n");
	for (int i = 0; i < (HASH_SIZE * size); i++) {
		printf("%d key:%d value: %d\n", i, hash[i].key, hash[i].value);
	}
	printf("--------------------------------------------\n");

	for (int i = 0; i < 40; i++) {
		int randKey = rand() % 50;
		if (search(hash, randKey, size) != -1) {
			printf("search key:%d value: %d\n", randKey, search(hash, randKey, size));
		}
	}
	printf("--------------------------------------------\n");

	for (int i = 0; i < 40; i++) {
		int randKey = rand() % 50;
		if (search(hash, randKey, size) == -1) {
			continue;
		}
		printf("delete key:%d value: %d\n", randKey, delete(hash, randKey, size));
	}

	printf("--------------------------------------------\n");
	for (int i = 0; i < (HASH_SIZE * size); i++) {
		printf("%d key:%d value: %d\n", i, hash[i].key, hash[i].value);
	}

	return 0;
}

element *reset (element *hash, int *size, double *density) {
	*size += 2;
	element *newHash = malloc (sizeof(element) * HASH_SIZE * (*size));

	for (int i = 0; i < HASH_SIZE * (*size); i++) {
		newHash[i].key = newHash[i].value = -1;
	}

	*density = 0;

	for (int i = 0; i < HASH_SIZE * ((*size)-2); i++) {
		if (hash[i].key != -1) {
			insert(&newHash, hash[i], size, density);
		}
	}

	free(hash);

	return newHash;
}

void insert (element **hash, element data, int *size, double *density) {
	int pos = data.key % (HASH_SIZE * (*size));
	while ((*hash)[pos].key != -1) {
		pos = (pos + 1) % (HASH_SIZE * (*size));
	}
	(*hash)[pos] = data;
	*density += (double) 1 / (HASH_SIZE * (*size));
	if (*density >= (double) 3 / 4) {
		*hash = reset(*hash, size, density);
	}
}

int search (element *hash, int key, int size) {
	int pos = key % (HASH_SIZE * size);
	
	while (hash[pos].key != -1) {
		if (hash[pos].key == key) {
			return hash[pos].value;
		}
		pos = (pos + 1) % (HASH_SIZE * size);
	}

	return -1;
}

int delete (element *hash, int key, int size) {
	int pos = key % (HASH_SIZE * size);
	int value;
	
	// find position
	while (hash[pos].key != -1) {
		if (hash[pos].key == key) {
			value = hash[pos].value;
			break;
		}
		pos = (pos + 1) % (HASH_SIZE * size);
	}
	hash[pos].key = -1;
	hash[pos].value = -1;
	
	// adjust
	int empty = pos;
	pos = (pos + 1) % (HASH_SIZE * size);
	while (hash[pos].key != -1) {
		if (hash[pos].key % (HASH_SIZE * size) != pos && (pos - (hash[pos].key % (HASH_SIZE * size)) + HASH_SIZE * size) % (HASH_SIZE * size) >= (pos - empty + HASH_SIZE * size) % (HASH_SIZE * size) ) {
			hash[empty].key = hash[pos].key;
			hash[empty].value = hash[pos].value;
			hash[pos].key = -1;
			hash[pos].value = -1;
			empty = pos;
		}
		pos = (pos + 1) % (HASH_SIZE * size);
	}
	return value;
}


