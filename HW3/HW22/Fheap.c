#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define tableSize 20
#define true 1
#define false 0

typedef int bool;

typedef struct fheap {
	struct fheap *rightSibling, *leftSibling, *child, *parent;
	int value, degree, key;
	bool childCut;
}fheap;

fheap *create (int, int);
void insert (int, int);
void meldHeight ();
void cut(fheap *);
fheap *findNode(fheap *, int, int);
fheap *extract ();
void delete (int, int);
void decrease (int, int, int);

fheap *min = NULL;
	
int main () {
	char *op = malloc (sizeof(char) * 10);
	int key, value, decreaseKey;

	while(true) {
		scanf("%s", op);

		if (!strcmp(op, "insert")) {
			scanf("%d %d", &key, &value);
			insert(key, value);		
		}
		else if (!strcmp(op, "extract")) {
			if (min != NULL) {
				fheap *temp = extract();
				printf("(%d)%d\n", temp->key, temp->value);
			}
		}
		else if (!strcmp(op, "delete")) {
			scanf("%d %d", &key, &value);
			delete(key, value);
		}
		else if (!strcmp(op, "decrease")) {
			scanf("%d %d %d", &key, &value, &decreaseKey);
			decrease(key, value, decreaseKey);
		}
		else {
			break;
		}

	}

	return 0;
}

fheap *create (int key, int value) {
	fheap *new = malloc (sizeof(fheap));
	new->rightSibling = new;
	new->leftSibling = new;
	new->child = NULL;
	new->parent = NULL;
	new->key = key;
	new->value = value;
	new->degree = 0;
	new->childCut = false;
	return new;
}

void insert (int key, int value) {
	if (min) {
		fheap *new = create(key, value);
		new->rightSibling = min->rightSibling;
		new->leftSibling = min;
		min->rightSibling->leftSibling = new;
		min->rightSibling = new;
	}
	else {
		min = create(key, value);
	}
}

fheap *extract () {
	fheap *temp = min;
	if (min->rightSibling != min && min->child) {
		// change min->child
		min = temp->child;	
		do {
			min->parent = NULL;
			min->childCut = false;
			min = min->rightSibling;
		} while (min != temp->child);
		min = min->leftSibling;

		temp->rightSibling->leftSibling = temp->leftSibling;
		temp->leftSibling->rightSibling = temp->rightSibling;

		min->rightSibling = temp->rightSibling->rightSibling;
		temp->child->leftSibling = temp->rightSibling;
		temp->rightSibling->rightSibling->leftSibling = min;
		temp->rightSibling->rightSibling = temp->child;
	}
	else if (min->rightSibling == min && min->child) {
		min = min->child;
		do {
			min->parent = NULL;
			min->childCut = false;
			min = min->rightSibling;
		} while (min != temp->child);
	}
	else if (min->rightSibling != min && !min->child) {
		temp->rightSibling->leftSibling = temp->leftSibling;
		temp->leftSibling->rightSibling = temp->rightSibling;
		min = temp->rightSibling;
	}
	else {
		min = NULL;
	}
	
	if (min) {
		meldHeight();
	}

	return temp;
}

void meldHeight () {
	fheap **table = calloc (tableSize, sizeof(fheap *));
	fheap *temp1 = min, *temp2, *temp3;

	do {
		temp2 = min->rightSibling;

		if (temp2 == min) {
			temp2 = NULL;
		}

		min->rightSibling->leftSibling = min->leftSibling;
		min->leftSibling->rightSibling = min->rightSibling;

		// table root point to itself
		min->rightSibling = min;
		min->leftSibling = min;

		while (table[min->degree]) {
			// if table->data < min->data then swap min and table element
			if (table[min->degree]->key < min->key) {
				temp3 = min;
				min = table[min->degree];
				table[min->degree] = temp3;
			}

			if (min->child) {
				table[min->degree]->rightSibling = min->child->rightSibling;
				table[min->degree]->leftSibling = min->child;
				min->child->rightSibling->leftSibling = table[min->degree];
				min->child->rightSibling = table[min->degree];
			}
			else {
				min->child = table[min->degree];
			}
			table[min->degree]->parent = min;
			table[min->degree] = NULL;
			min->degree++;
		}
		table[min->degree] = min;
		min = temp2;
	} while (min);

	for (int i = 0; i < tableSize; i++) {
		if (table[i]) {
			if (min) {
				table[i]->rightSibling = min->rightSibling;
				table[i]->leftSibling = min;
				min->rightSibling->leftSibling = table[i];
				min->rightSibling = table[i];
				if (table[i]->key < min->key) {
					min = table[i];
				}
			}
			else {
				min = table[i]; 
				min->rightSibling = min; 
				min->leftSibling = min;
			}
		}
	}
}

fheap *findNode(fheap *root, int key, int value) {
	fheap *temp = root;
	do {
		if (temp->key == key && temp->value == value) {
			return temp;
		}
		if (temp->child) {
			fheap *temp1 = findNode(root->child, key, value);
			if (temp1) {
				return temp1;
			}
		}
		temp = temp->rightSibling;
	}while (temp->rightSibling != root);
	return NULL;
}

void cut(fheap *root) {
	while (root->childCut) {
		fheap *temp = root->parent;
		root->childCut = false;
		root->parent = NULL;

		if (temp->degree == 1) {
			temp->child = NULL;
		}
		else {
			root->rightSibling->leftSibling = root->leftSibling;
			root->leftSibling->rightSibling = root->rightSibling;
		}
		temp->degree--;

		root->rightSibling = min->rightSibling;
		root->leftSibling = min;
		min->rightSibling->leftSibling = root;
		min->rightSibling = root;

		root = temp;
	}

	if (root->parent) {
		root->childCut = true;
	}
}

void delete (int key, int value) {	
	fheap *target = findNode(min, key, value);
	if (min == target) {
		min = target->rightSibling;
	}

	if (target->child) {
		fheap *temp = target->child;
	
		do {
			temp->parent = NULL;
			temp->childCut = false;
			temp = temp->rightSibling;
		} while (temp != target->child);
	
		temp = temp->leftSibling;
		temp->rightSibling = min->rightSibling;
		target->child->leftSibling = min;
		min->rightSibling->leftSibling = temp;
		min->rightSibling = target->child;
	}

	target->rightSibling->leftSibling = target->leftSibling;
	target->leftSibling->rightSibling = target->rightSibling;

	if (target->parent) {
		if (target->parent->degree == 1) {
			target->parent->child = NULL;
		}
		if (target->parent->child = target) {
			target->parent->child = target->rightSibling;
		}
		target->parent->degree--;

		cut(target->parent);
	}
}

void decrease (int key, int value, int decreaseKey) {
	fheap *target = findNode(min, key, value);

	target->key -= decreaseKey;

	if (target->parent && target->value < target->parent->value) {
		fheap *parent = target->parent;

		target->parent = NULL;
		if (parent->degree == 1) {
			parent->child = NULL;
		}
		else {
			target->rightSibling->leftSibling = target->leftSibling;
			target->leftSibling->rightSibling = target->rightSibling;
		}
		parent->degree--;

		target->rightSibling = min->rightSibling;
		target->leftSibling = min;
		min->rightSibling->leftSibling = target;
		min->rightSibling = target;
		
		cut(parent);	
	}

	if (target->key < min->key) {
		min = target;
	}
}
