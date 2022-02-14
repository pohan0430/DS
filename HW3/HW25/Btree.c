#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define true 1
#define false 0

typedef short int bool;

typedef struct Btree {
	struct Btree *A0, *A1, *A2;
	int E1, N1, E2, N2;
}Btree;

Btree *root = NULL;
int replaceNum = -1;

Btree *create (int);
Btree *split (Btree *, Btree *, int);
Btree *insert (Btree *, int);
int get (int);
void combine (Btree *, int, bool *);
bool rotate (Btree *, int);
bool delete (Btree *, int, bool, bool *);
int getk (Btree *, int);
int deletek (Btree *, int, bool, bool *);

int main () {
	char op[10];
	int element, opNum;

	scanf("%d", &opNum);

	for (int i = 0; i < opNum; i++) {
		scanf("%s %d", op, &element);

		if (!strcmp(op, "add")) {
			insert(root, element);
			printf("add(%d) = ok\n", element);
		}
		else if (!strcmp(op, "get")) {
			int key = get(element);
			if (key < 0) {
				printf("get(%d) = not found\n", element);
			}
			else {
				printf("get(%d) = %d\n", element, key);
			}
		}
		else if (!strcmp(op, "getk")) {
			int key = getk(root, element);
			if (key < 0) {
				printf("getk(%d) = not found\n", element);
			}
			else {
				printf("getk(%d) = %d\n", element, key);
			}

		}
		else if (!strcmp(op, "remove")) {
			bool unbalance = true;
			if (delete(root, element, false, &unbalance)) {
				printf("remove(%d) = %d\n", element, element);
			}
			else {
				printf("remove(%d) = not found\n", element);
			}
		}
		else {
			bool unbalance = true;
			int key = deletek(root, element, false, &unbalance);
			if (key < 0) {
				printf("removek(%d) = not found\n", element);
			}
			else {
				printf("removek(%d) = %d\n", element, key);
			}
		}
	}
	
	return 0;
}

Btree *create (int element) {
	Btree *node = malloc (sizeof(Btree));
	node->E1 = element;
	node->E2 = INT_MAX;
	node->A0 = node->A1 = node->A2 = NULL;
	node->N1 = 1;
	node->N2 = 2;
	return node;
}

Btree *split (Btree *target, Btree *key, int keyOrder) {
	Btree *parent, *child, *temp;
	int A2Num = 0;
	temp = target->A2;
	while (temp != NULL) {
		if (temp->E2 == INT_MAX) {
			A2Num += (temp->N2 - 1);
			break;
		}
		A2Num += temp->N2;
		temp = temp->A2;
	}
	switch (keyOrder) {
		case 0:
			parent = create(target->E1);
			child = create(target->E2);
			parent->A1 = child;
			child->A0 = target->A1;
			child->A1 = target->A2;
			child->N1 = target->N2 - target->N1;
			child->N2 = child->N1 + A2Num + 1;
			parent->N2 = parent->N1 + child->N2;
			target->E1 = key->E1;
			target->E2 = INT_MAX;
			target->A1 = key->A1;
			target->A2 = NULL;
			if (target->A0 != NULL) {
				target->N1 = target->A0->N2;
			}
			target->N2 = target->N1 + key->N2 - key->N1;
			break;
		case 1:
			parent = create(key->E1);
			child = create(target->E2);
			child->A0 = key->A1;
			child->A1 = target->A2;
			parent->A1 = child;
			child->N1 = key->N2 - key->N1;
			child->N2 = child->N1 + 1 + A2Num;
			parent->N2 = parent->N1 + child->N2;
			target->E2 = INT_MAX;
			target->A2 = NULL;
			if (target->A1 != NULL) {
				target->N2 = target->N1 + target->A1->N2;
			}
			break;
		case 2:
			parent = create(target->E2);
			child = key;
			child->A0 = target->A2;
			parent->A1 = child;
			int n1 = child->N1;
			int n2 = child->N2;
			child->N1 = A2Num + 1;
			child->N2 = child->N1 + n2 - n1;
			parent->N2 = parent->N1 + child->N2;
			target->E2 = INT_MAX;
			target->A2 = NULL;
			break;
	}
	if (target == root) {
		parent->A0 = target;
		parent->N2 -= parent->N1;
		parent->N1 = target->N2;
		parent->N2 += parent->N1;
		root = parent;
	}
	return parent;
}

Btree *insert (Btree *target, int key) {
	if (root == NULL) {
		Btree *new = create(key);
		root = new;
		return NULL;
	}

	if (key < target->E1) {
		if (target->A0 == NULL) {
			if (target->E2 == INT_MAX) {
				target->N2 = 2;
				target->E2 = target->E1;
				target->E1 = key;
			}
			else {
				Btree *new = create(key);
				return split(target, new, 0);
			}
		}
		else {
			Btree *new = insert(target->A0, key);
			if (new != NULL) {
				if (target->E2 == INT_MAX) {
					target->E2 = target->E1;
					target->A2 = target->A1;
					target->E1 = new->E1;
					target->A1 = new->A1;
					target->N1 = target->A0->N2;
					target->N2 = target->N1 + new->N2 - new->N1;
				}
				else {
					return split(target, new, 0);
				}	
			}
			else {
				target->N1 += 1;
				target->N2 += 1;
			}
		}	
	}
	else if (key < target->E2) {
		if (target->A1 == NULL) {
		       	if (target->E2 == INT_MAX) {
				target->N2 = 2;
				target->E2 = key;
			}
			else {
				Btree *new = create(key);
				return split(target, new, 1);
			}
		}
		else {
			Btree *new = insert(target->A1, key);
			if (new != NULL) {
				if (target->E2 == INT_MAX) {
					target->E2 = new->E1;
					target->A2 = new->A1;
					target->N2 = target->N1 + target->A1->N2;
				}
				else {
					return split(target, new, 1);
				}
			}
			else {
				target->N2 += 1;
			}
		}
	}
	else {
		if (target->A2 == NULL) {
			Btree *new = create(key);
			return split(target, new, 2);
		}
		else {
			Btree *new = insert(target->A2, key);
			if (new != NULL) {
				return split(target, new, 2);
			}
		}
	}
	return NULL;
}

int get (int key) {
	Btree *target = root;
	if (target == NULL) {
		return -1;
	}

	while (1) {
		if (key < target->E1) {
			if (target->A0 == NULL) {
				return -1;
			}
			else {
				target = target->A0;
			}
		}
		else if (key == target->E1) {
			return key;
		}
		else if (key < target->E2) {
			if (target->A1 == NULL) {
				return -1;
			}
			else {
				target = target->A1;
			}
		}
		else if (key == target->E2) {
			return key;
		}
		else {
			if (target->A2 == NULL) {
				return -1;
			}
			else {
				target = target->A2;
			}
		}
	}
}

void combine (Btree *target, int unbalancedir, bool *unbalance) {
	switch (unbalancedir) {
		case 0:
			if (target->E2 != INT_MAX) {
				target->A1->E2 = target->A1->E1;
				int A2Num = 0;
				Btree *temp = target->A2;
				while (temp != NULL) {
					if (temp->E2 == INT_MAX) {
						A2Num += (temp->N2 - 1);
						break;
					}
					A2Num += temp->N2;
					temp = temp->A2;
				}
				target->A1->E1 = target->E1;
				target->E1 = target->E2;
				target->E2 = INT_MAX;
				target->A1->A2 = target->A1->A1;
				target->A1->A1 = target->A1->A0;
				target->A1->A0 = target->A0->A0;
				int m1 = target->A1->N1;
				target->A1->N1 = target->A0->N1;
				target->A1->N2 = target->A1->N1 + m1;
				target->A0 = target->A1;
				target->A1 = target->A2;
				target->A2 = NULL;
				target->N1 = target->N2 - 1;
				target->N2 += A2Num;
				if (target->A2 != NULL) {
					target->N2 += target->A2->N2;
				}
				*unbalance = false;
			}
			else {
				target->A1->E2 = target->A1->E1;
				target->A1->E1 = target->E1;
				target->A1->A2 = target->A1->A1;
				target->A1->A1 = target->A1->A0;
				target->A1->A0 = target->A0->A0;
				int m1 = target->A1->N1;
				target->A1->N1 = target->A0->N1;
				target->A1->N2 = target->A1->N1 + m1;
				// combine put in leftist child
				target->A0 = target->A1;
				target->A1 = NULL;
				target->E1 = INT_MAX;
				target->N1 = target->N2 - 1;
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
				int n1 = target->A2->N1;
				int n2 = target->A2->N2;
				target->A2->N1 = target->A1->N1;
				target->A2->N2 = target->A2->N1 + n1;
				target->N2 = target->N1 + target->A2->N1 + n2;
				target->A1 = target->A2;
				target->A2 = NULL;
				*unbalance = false;
			}
			else {
				target->A0->E2 = target->E1;
				target->E1 = INT_MAX;
				target->A0->A2 = target->A1->A0;
				target->N1 = target->N2 - 1;
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
			target->N2 = target->N1 + target->A1->N2 + target->A2->N1;
			target->A2 = NULL;
			*unbalance = false;
			break;
	}
}

bool rotate (Btree *target, int unbalancedir) {
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
				target->A0->N2 = target->A0->N1 + target->A1->N1;
				target->A1->N1 = target->A1->N2 - target->A1->N1;
				target->A1->N2 = target->N2 - target->N1 - target->A1->N2 + target->A1->N1;
				target->N1 = target->A0->N2;
				target->N2 -= 1;
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
				int m1 = target->A1->N1;
				target->A1->N1 = target->N1 - target->A0->N2;
				target->A1->N2 = target->A1->N1 + m1;
				target->N1 = target->A0->N2;
				target->N2 -= 1;
				return true;
			}
			if (target->A2 != NULL) {
				if (target->A2->E2 != INT_MAX) {
					int A2Num = 0;
					if (target->A2->A2 != NULL) {
						Btree *temp = target->A2->A2;
						while (temp != NULL) {
							if (temp->E2 == INT_MAX) {
								A2Num += (temp->N2 - 1);
								break;
							}
							A2Num += temp->N2;
							temp = temp->A2;
						}
					}
					target->A1->E1 = target->E2;
					target->E2 = target->A2->E1;
					target->A2->E1 = target->A2->E2;
					target->A2->E2 = INT_MAX;
					target->A1->A1 = target->A2->A0;
					target->A2->A0 = target->A2->A1;
					target->A2->A1 = target->A2->A2;
					target->A2->A2 = NULL;
					int m1 = target->A2->N1;
					target->A1->N2 = target->A1->N1 + target->A2->N1;
					target->A2->N1 = target->A2->N2 - target->A2->N1;
					target->A2->N2 = target->A2->N1 + A2Num + 1;
					target->N2 = target->N1 + target->A1->N2;
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
				int m1 = target->A2->N1;
				target->A2->N1 = target->N2 - target->A1->N2 - target->N1;
				target->A2->N2 = target->A2->N1 + m1;
				target->N2 = target->N1 + target->A1->N2;
				return true;
			}
			break;
	}
	return false;
}

bool delete (Btree *target, int key, bool flag, bool *unbalanced) {
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
					target->N1 = 1;
					target->N2 = 2;
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
				target->N2 = 2;
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
	else if (find == true) {
		// adjust N
		switch (unbalancedir) {
			case 0:
				target->N1 -= 1;
			case 1:
				target->N2 -= 1;
				break;
			default:
				break;
		}
	}

	return find;
}

int getk (Btree *target, int k) {
	while (1) {
		if (target == NULL || k < 1) {
			return -1;
		}
		else if (k < target->N1) {
			target = target->A0;
		}
		else if (k == target->N1) {
			return target->E1;
		}
		else if (k < target->N2) {
			k -= target->N1;
			target = target->A1;
		}
		else if (k == target->N2) {
			if (target->E2 == INT_MAX) {
				return -1;
			}
			return target->E2;
		}
		else {
			k -= target->N2;
			target = target->A2;
		}
	}
}

int deletek (Btree *target, int k, bool flag, bool *unbalanced) {
	if (target == NULL || k < 1) {
		*unbalanced = false;
		return -1;
	}
	int find = -1;
	int unbalancedir;
	
	if (!flag) {
		// find delete key
		if (k < target->N1) {
			find = deletek(target->A0, k, false, unbalanced);
			unbalancedir = 0;
		}
		else if (k == target->N1) {
			find = target->E1;
			target->E1 = INT_MAX;
			if (target->A1 != NULL) {
				// delete from internal node
				deletek(target->A1, k, true, unbalanced);
				target->E1 = replaceNum;
				unbalancedir = 1;
			}
			else {
				// delete from a leaf
				if (target->E2 != INT_MAX) {
					*unbalanced = false;
					target->E1 = target->E2;
					target->E2 = INT_MAX;
					target->N1 = 1;
					target->N2 = 2;
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
		else if (k < target->N2) {
			k -= target->N1;
			find = deletek(target->A1, k, false, unbalanced);
			unbalancedir = 1;
		}
		else if (k == target->N2) {
			if (target->E2 == INT_MAX) {
				// not found
				*unbalanced = false;
				return find;
			}
			find = target->E2;
			target->E2 = INT_MAX;
			if (target->A2 != NULL) {
				// delete from internal node
				deletek(target->A2, k, true, unbalanced);
				target->E2 = replaceNum;
				unbalancedir = 2;
			}
			else {
				// delete from a leaf
				target->E2 = INT_MAX;
				*unbalanced = false;
				target->N2 = 2;
				// root = NULL
				if (root->E1 == INT_MAX && root->E2 == INT_MAX) {
					root = NULL;
				}
				return find;
			}
		}
		else {
			k -= target->N2;
			find = deletek(target->A2, k, false, unbalanced);
			unbalancedir = 2;
		}
	}
	else {
		find = true;
		// find smallest key to replace delete
		if (target->A0 != NULL) {
			deletek(target->A0, k, true, unbalanced);
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
	else if (find >= 0) {
		// adjust N
		switch (unbalancedir) {
			case 0:
				target->N1 -= 1;
			case 1:
				target->N2 -= 1;
				break;
			default:
				break;
		}
	}

	return find;
}

