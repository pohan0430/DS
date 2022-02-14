#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct patricia {
	int value, bit;
	char key[1010];
	struct patricia *leftChild, *rightChild;
}patricia;

bool insert (patricia **, char *, int);
patricia *search (patricia *, char *);
int delete (patricia **, char *);

int main () {
	int key_length, value;
	char op[10];
	scanf("%d", &key_length);
	char key[key_length];
	
	patricia *head = NULL;
	
	while (1) {
		scanf("%s", op);

		if (!strcmp(op, "insert")) {
			scanf("%s %d", key, &value);
			if (insert(&head, key, value)) {
				printf("insert -> %d\n", value);
			}
			else {
				printf("insert -> conflict\n");
			}
		}
		else if (!strcmp(op, "search")) {
			scanf("%s", key);
			patricia *target = search(head, key);
			if (target == NULL || strcmp(target->key, key) != 0) {
				printf("search -> not found\n");
			}
			else {
				printf("search -> %d\n", target->value);
			}
		}
		else if (!strcmp(op, "delete")) {
			scanf("%s", key);
			value = delete(&head, key);
			if (value == -1) {
				printf("delete -> not found\n");
			}
			else {
				printf("delete -> %d\n", value);
			}
			
		}
		else {
			break;
		}
	}

	return 0;
}

bool insert (patricia **head, char *key, int value) {
	patricia *current, *parent, *lastNode, *newNode;
	if (*head == NULL) {
		*head = malloc (sizeof(patricia));
		strcpy((*head)->key, key);
		(*head)->leftChild = *head;
		(*head)->rightChild = NULL;
		(*head)->bit = -1;
		(*head)->value = value;
	}
	else {
		// find the most closest prefix key
		lastNode = search((*head), key);
		if (!strcmp(lastNode->key, key)) {
			return false;
		}
		
		// find the first different bit from left to right
		int i;
		for (i = 0; lastNode->key[i] == key[i]; i++);

		// search tree using the first i-1 bits
		current = (*head)->leftChild;
		parent = (*head);
		while (current->bit > parent->bit && current->bit < i) {
			parent = current;
			current = key[current->bit] == '1' ? current->rightChild : current->leftChild;
		}

		// insert the element as a child of parent
		patricia *newNode = malloc (sizeof(patricia));
		newNode->value = value;
		newNode->bit = i;
		strcpy(newNode->key, key);
		newNode->leftChild = key[newNode->bit] == '1' ? current : newNode;
		newNode->rightChild = key[newNode->bit] == '1' ? newNode : current;
		if (current == parent->leftChild) {
			parent->leftChild = newNode;
		}
		else {
			parent->rightChild = newNode;
		}
	}
	return true;
}

patricia *search (patricia *head, char *key) {
	if (head == NULL) {
		return NULL;
	}
	else {
		patricia *currentNode = head, *nextNode = head->leftChild;
		while (nextNode->bit > currentNode->bit) {
			currentNode = nextNode;
			nextNode = key[currentNode->bit] == '1' ? currentNode->rightChild : currentNode->leftChild;
		}
		return nextNode;
	}
}

int delete (patricia **head, char *key) {
	if (*head == NULL) {
		return -1;
	}
	else {
		int value;
		patricia *grandParentNode = NULL, *parentNode = *head, *currentNode = (*head)->leftChild;

		// find delete key
		while (currentNode->bit > parentNode->bit) {
			grandParentNode = parentNode; 
			parentNode = currentNode;
			currentNode = key[currentNode->bit] == '1' ? currentNode->rightChild : currentNode->leftChild;
		}
		
		// delete key is not exist 
		if (strcmp(currentNode->key, key) != 0) {
			return -1;
		}
		
		value = currentNode->value;
		if (currentNode->leftChild == currentNode) {
			// if delete node has one self pointer
			if (currentNode == *head) {
				*head = NULL;
			}
			else if (grandParentNode->leftChild == parentNode) {
				grandParentNode->leftChild = currentNode->rightChild;
			}
			else {
				grandParentNode->rightChild = currentNode->rightChild;
			}
			free(currentNode);
		}
		else if (currentNode->rightChild == currentNode) {
			// if delete node has one self pointer
			if (grandParentNode->leftChild == parentNode) {
				grandParentNode->leftChild = currentNode->leftChild;
			}
			else {
				grandParentNode->rightChild = currentNode->leftChild;
			}
			free(currentNode);
		}
		else {
			// if delete node has no self pointer
			char key_q[1010];
			strcpy(key_q, parentNode->key);
			patricia *r = parentNode, *q = key_q[parentNode->bit] == '1' ? parentNode->rightChild : parentNode->leftChild;
			while (q->bit > r->bit) {
				r = q;
				q = key_q[q->bit] == '1' ? q->rightChild : q->leftChild;
			}
			
			strcpy(currentNode->key, parentNode->key);

			if (r->leftChild == q) {
				r->leftChild = currentNode;
			}
			else {
				r->rightChild = currentNode;
			}

			if (parentNode->rightChild == currentNode) {
				if (grandParentNode->leftChild == parentNode) {
					grandParentNode->leftChild = parentNode->leftChild;
				}
				else {
					grandParentNode->rightChild = parentNode->leftChild;
				}
			}
			else {
				if (grandParentNode->leftChild == parentNode) {
					grandParentNode->leftChild = parentNode->rightChild;
				}
				else {
					grandParentNode->rightChild = parentNode->rightChild;
				}
			}
			free(parentNode);
		}

		return value;
	}

}
