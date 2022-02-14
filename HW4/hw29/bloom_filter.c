#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>

typedef int element;

typedef struct chain {
	element key;
	struct chain *next;
}chain;

int hashFunction1 (element);
int hashFunction2 (element);
int hashFunction3 (element);
int hashFunction4 (element);
void insert (chain **, element);
bool search (chain **, element);
bool search_error (chain **, element);
double P (double);

int TABLE_SIZE = 8191;
int UPDATES = 2047;
int SEARCH = 100000;
int h = 1;
int n = INT_MAX;

int main () {
	srand(time(NULL));
	int error[4] = {0};
	for (; h <= 4; h++) {
		chain **hashTable = calloc (sizeof(chain *), TABLE_SIZE);
	
		// UPDATES
		for (int i = 0; i < UPDATES; i++) {
			element randKey = rand();
			if (search(hashTable, randKey)) {
				continue;
			}	
			insert(hashTable, randKey); 
		}
	
		// SEARCH
		for (int i = 0; i < SEARCH; i++) {
			element randKey = rand();
			if (search_error(hashTable, randKey)) {
				error[h-1]++;
			}
		}
		free(hashTable);
	}

	printf("filter size: %d\n", TABLE_SIZE);
	printf("number of master file records: 2^(31)-1\n");
	printf("number of updates: %d\n", UPDATES);
	printf("number of searches: %d\n", SEARCH);
	printf("optimal h: %.3lf\n", 0.693 * (double) TABLE_SIZE / (double) UPDATES);
	for (int i = 1; i <= 4; i++) {
		printf("---------------------------------------\n");
		printf("number of hash function: %d\n", i);
		printf("error times: %d\n", error[i-1]);
		printf("probability of error: %lf\n", (double) error[i-1] / SEARCH);
		printf("probability of ideal error: %lf\n", P(i));
	}

	return 0;
}

int hashFunction1 (element key) {
	key %= 32763;
	key = (key * key >> 3) % TABLE_SIZE;
	return key;
}

int hashFunction2 (element key) {
	key = key % TABLE_SIZE;
	return key;
}

int hashFunction3 (element key) {
	key %= 1073741821;
	key = (key * 2 >> 5) % TABLE_SIZE;
	return key;
}

int hashFunction4 (element key) {
	key %= 429496727;
	key = (key * 5 >> 7) % TABLE_SIZE;
	return key;
}

void insert (chain **hashTable, element key) {
	int pos;
	chain *newNode;
	switch (h) {
		case 4:
			pos = hashFunction4(key);
			newNode = (chain *) malloc (sizeof(chain));
			newNode->key = key;
			newNode->next = hashTable[pos];
			hashTable[pos] = newNode;
		case 3:
			pos = hashFunction3(key);
			newNode = (chain *) malloc (sizeof(chain));
			newNode->key = key;
			newNode->next = hashTable[pos];
			hashTable[pos] = newNode;
		case 2:
			pos = hashFunction2(key);
			newNode = (chain *) malloc (sizeof(chain));
			newNode->key = key;
			newNode->next = hashTable[pos];
			hashTable[pos] = newNode;
		case 1:
			pos = hashFunction1(key);
			newNode = (chain *) malloc (sizeof(chain));
			newNode->key = key;
			newNode->next = hashTable[pos];
			hashTable[pos] = newNode;
			break;
	}

}

bool search (chain **hashTable, element key) {
	int pos = hashFunction1(key);

	chain *p = hashTable[pos];
	while (p) {
		if (p->key == key) {
			return true;
		}
		else {
			p = p->next;
		}
	}
	return false;
}

bool search_error (chain **hashTable, element key) {
	int pos;
	chain *p;
	switch (h) {
		case 4:
			pos = hashFunction4(key);

			p = hashTable[pos];
			if (!p) {
				return false;
			}
			while (p) {
				if (p->key == key) {
					return false;
				}
				p = p->next;
			}
		case 3:
			pos = hashFunction3(key);

			p = hashTable[pos];
			if (!p) {
				return false;
			}
			while (p) {
				if (p->key == key) {
					return false;
				}
				p = p->next;
			}
		case 2:
			pos = hashFunction2(key);

			p = hashTable[pos];
			if (!p) {
				return false;
			}
			while (p) {
				if (p->key == key) {
					return false;
				}
				p = p->next;
			}
		case 1:
			pos = hashFunction1(key);
	
			p = hashTable[pos];
			if (!p) {
				return false;
			}
			while (p) {
				if (p->key == key) {
					return false;
				}
				p = p->next;
			}
			break;
	}
	return true;
}

double P (double h) {
    return pow(1 - (double)1 / n, UPDATES) * pow(1 - pow(1 - 1 / (double)TABLE_SIZE, (double)UPDATES * h), h);
}
