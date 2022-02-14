#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define true 1
#define false 0

typedef short int bool;

typedef struct BPlusTree {
	struct BPlusTree *A0, *A1, *A2;
	int E1, N1;
}BPlustree;

BPlustree *create (int);
BPlusTree *split (BPlusTree *, BPlusTree *, int);
BPlusTree *insert (BPlusTree *, int);
void combine (BPlusTree *, int, bool *);
bool rotate (BPlusTree *, int);
bool delete (BPlusTree *, int, bool, bool *);
int *rangeSearch(BPlusTree *, int ,int);

BPlustree *root = NULL;

int main () {
	/*
	srand(time(NULL));
	int element = 7;
 	int data[element], del[element/2];
	while (1) {
        	for (int i = 0; i < element; i++)
         	{
            		data[i] = rand() % 50;
			insert(root, data[i]);
            		printf("%d ", data[i]);
        	}
        	printf("\n");
		for (int i = 0; i < element / 2; i++) {
			bool unbalance = true;
			del[i] = data[rand() % element];
			printf("#%d\n", del[i]);
			delete(root, del[i], false, &unbalance);
		}
		root = NULL;
    	}
	*/
	
	FILE *fpr = fopen("input_01.txt", "r");
	char op[10];
	int key1, key2, opNum;

	fscanf(fpr, "%d", &opNum);

	for (int i = 0; i < opNum; i++) {
		fscanf(fpr, "%s", op);

		if (!strcmp(op, "add")) {
			scanf("%d", &key1);
			insert(root, key1);
		}
		else if (!strcmp(op, "remove")) {
			scanf("%d", &key1);
			bool unbalance = true;
			delete(root, key1, false, &unbalance);
		}
		else {
			scanf("%d %d", &key1, &key2);
			int *num = rangeSearch(root, key1, key2);
		}
	}
	
	return 0;
}

BPlusTree *create (int element) {
	BPlusTree *node = malloc (sizeof(BPlusTree));
	node->E1 = element;
	node->E2 = INT_MAX;
	node->A0 = node->A1 = node->A2 = NULL;
	return node;
}

BPlusTree *split (BPlusTree *target, BPlusTree *key, int keyOrder) {
	BPlusTree *parent, *child, *temp;
	switch (keyOrder) {
		case 0:
			parent = create(target->E1);
			child = create(target->E2);
			parent->A1 = child;
			child->A0 = target->A1;
			child->A1 = target->A2;
			target->E1 = key->E1;
			target->E2 = INT_MAX;
			target->A1 = key->A1;
			target->A2 = NULL;
			break;
		case 1:
			parent = create(key->E1);
			child = create(target->E2);
			child->A0 = key->A1;
			child->A1 = target->A2;
			parent->A1 = child;
			target->E2 = INT_MAX;
			target->A2 = NULL;
			break;
		case 2:
			parent = create(target->E2);
			child = key;
			child->A0 = target->A2;
			parent->A1 = child;
			target->E2 = INT_MAX;
			target->A2 = NULL;
			break;
	}
	if (target == root) {
		parent->A0 = target;
		root = parent;
	}
	return parent;
}

BPlusTree *insert (BPlusTree *target, int key) {
	if (root == NULL) {
		BPlusTree *new = create(key);
		root = new;
		return NULL;
	}

	if (key < target->E1) {
		if (target->A0 == NULL) {
			if (target->E2 == INT_MAX) {
				target->E2 = target->E1;
				target->E1 = key;
			}
			else {
				BPlusTree *new = create(key);
				return split(target, new, 0);
			}
		}
		else {
			BPlusTree *new = insert(target->A0, key);
			if (new != NULL) {
				if (target->E2 == INT_MAX) {
					target->E2 = target->E1;
					target->A2 = target->A1;
					target->E1 = new->E1;
					target->A1 = new->A1;
				}
				else {
					return split(target, new, 0);
				}	
			}
		}	
	}
	else if (key < target->E2) {
		if (target->A1 == NULL) {
		       	if (target->E2 == INT_MAX) {
				target->E2 = key;
			}
			else {
				BPlusTree *new = create(key);
				return split(target, new, 1);
			}
		}
		else {
			BPlusTree *new = insert(target->A1, key);
			if (new != NULL) {
				if (target->E2 == INT_MAX) {
					target->E2 = new->E1;
					target->A2 = new->A1;
				}
				else {
					return split(target, new, 1);
				}
			}
		}
	}
	else {
		if (target->A2 == NULL) {
			BPlusTree *new = create(key);
			return split(target, new, 2);
		}
		else {
			BPlusTree *new = insert(target->A2, key);
			if (new != NULL) {
				return split(target, new, 2);
			}
		}
	}
	return NULL;
}

void combine (BPlusTree *target, int unbalancedir, bool *unbalance) {
	switch (unbalancedir) {
		case 0:
			if (target->E2 != INT_MAX) {
				target->A1->E2 = target->A1->E1;
				target->A1->E1 = target->E1;
				target->E1 = target->E2;
				target->E2 = INT_MAX;
				target->A1->A2 = target->A1->A1;
				target->A1->A1 = target->A1->A0;
				target->A1->A0 = target->A0->A0;
				target->A0 = target->A1;
				target->A1 = target->A2;
				target->A2 = NULL;
				*unbalance = false;
			}
			else {
				target->A1->E2 = target->A1->E1;
				target->A1->E1 = target->E1;
				target->A1->A2 = target->A1->A1;
				target->A1->A1 = target->A1->A0;
				target->A1->A0 = target->A0->A0;
				// combine put in leftist child
				target->A0 = target->A1;
				target->A1 = NULL;
				target->E1 = INT_MAX;
				*unbalance = true;
				if (root == target) {
					root = target->A0;
					*unbalance = false;
				}
			}
			break;
		case 1:
			if (target->E2 != INT_MAX) {
				target->A2->E2 = target->A2->E1;
				target->A2->E1 = target->E2;
				target->E2 = INT_MAX;
				target->A2->A2 = target->A2->A1;
				target->A2->A1 = target->A2->A0;
				target->A2->A0 = target->A1->A0;
				target->A1 = target->A2;
				target->A2 = NULL;
				*unbalance = false;
			}
			else {
				target->A0->E2 = target->E1;
				target->E1 = INT_MAX;
				target->A0->A2 = target->A1->A0;
				target->A1 = NULL;
				*unbalance = true;
				if (root == target) {
					root = target->A0;
					*unbalance = false;
				}
			}
			break;
		case 2:
			target->A1->E2 = target->E2;
			target->E2 = INT_MAX;
			target->A1->A2 = target->A2->A0;
			target->A2 = NULL;
			*unbalance = false;
			break;
	}
}

bool rotate (BPlusTree *target, int unbalancedir) {
	switch (unbalancedir) {
		case 0:
			if (target->A1->E2 != INT_MAX) {
				target->A0->E1 = target->E1;
				target->E1 = target->A1->E1;
				target->A1->E1 = target->A1->E2;
				target->A1->E2 = INT_MAX;
				target->A0->A1 = target->A1->A0;
				target->A1->A0 = target->A1->A1;
				target->A1->A1 = target->A1->A2;
				target->A1->A2 = NULL;
				return true;
			}
			break;
		case 1:
			if (target->A0->E2 != INT_MAX) {
				target->A1->E1 = target->E1;
				target->E1 = target->A0->E2;
				target->A0->E2 = INT_MAX;
				target->A1->A1 = target->A1->A0;
				target->A1->A0 = target->A0->A2;
				target->A0->A2 = NULL;
				return true;
			}
			if (target->A2 != NULL) {
				if (target->A2->E2 != INT_MAX) {
					target->A1->E1 = target->E2;
					target->E2 = target->A2->E1;
					target->A2->E1 = target->A2->E2;
					target->A2->E2 = INT_MAX;
					return true;
				}
			}
			break;
		case 2:
			if (target->A1->E2 != INT_MAX) {
				target->A2->E1 = target->E2;
				target->E2 = target->A1->E2;
				target->A1->E2 = INT_MAX;
				target->A2->A1 = target->A2->A0;
				target->A2->A0 = target->A1->A2;
				target->A1->A2 = NULL;
				return true;
			}
			break;
	}
	return false;
}

bool delete (BPlusTree *target, int key, bool flag, bool *unbalanced) {
	bool find = false;
	int unbalancedir;
	if (target == NULL) {
		*unbalanced = false;
		return false;
	}

	if (!flag) {
		// find delete key
		if (key < target->E1) {
			find = delete(target->A0, key, false, unbalanced);
			unbalancedir = 0;
		}
		else if (key == target->E1) {
			target->E1 = INT_MAX;
			find = true;
			if (target->A1 != NULL) {
				// delete from internal node
				delete(target->A1, key, true, unbalanced);
				target->E1 = replaceNum;
				unbalancedir = 1;
			}
			else {
				// delete from a leaf
				if (target->E2 != INT_MAX) {
					*unbalanced = false;
					target->E1 = target->E2;
					target->E2 = INT_MAX;
				}
				else {
					target->E1 = INT_MAX;
				}
				// root = NULL
				if (root->E1 == INT_MAX && root->E2 == INT_MAX) {
					root = NULL;
				}
				return find;
			}
		}
		else if (key < target->E2) {
			find = delete(target->A1, key, false, unbalanced);
			unbalancedir = 1;
		}
		else if (key == target->E2) {
			target->E2 = INT_MAX;
			find = true;
			if (target->A2 != NULL) {
				// delete from internal node
				delete(target->A2, key, true, unbalanced);
				target->E2 = replaceNum;
				unbalancedir = 2;
			}
			else {
				// delete from a leaf
				target->E2 = INT_MAX;
				*unbalanced = false;
				// root = NULL
				if (root->E1 == INT_MAX && root->E2 == INT_MAX) {
					root = NULL;
				}
				return find;
			}
		}
		else {
			find = delete(target->A2, key, false, unbalanced);
			unbalancedir = 2;
		}
	}
	else {
		find = true;
		// find smallest key to replace delete 
		if (target->A0 != NULL) {
			delete(target->A0, key, true, unbalanced);
			unbalancedir = 0;
		}
		else {
			replaceNum = target->E1;
			if (target->E2 != INT_MAX) {
				target->E1 = target->E2;
				target->E2 = INT_MAX;
				*unbalanced = false;
			}
			else {
				target->E1 = INT_MAX;
			}
			return find;
		}
	}
	
	if (*unbalanced) {
		// adjust tree height
		if (rotate(target, unbalancedir)) {
			*unbalanced = false;
		}
		else {
			combine(target, unbalancedir, unbalanced);
		}
	}

	return find;
}

int *rangeSearch(BPlusTree *target, int low, int high) {

}
