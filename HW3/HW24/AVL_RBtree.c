#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define red 1
#define black 0

typedef short int bool;

typedef struct RBTree {
	int data;
	struct RBTree *rightChild, *leftChild;
	short int rightColor, leftColor;
}RBTree;

typedef struct AVL {
	struct AVL *rightChild;
	struct AVL *leftChild;
	int data, bf;
}AVL;

AVL *create_AVL (int);
AVL *leftRotation_AVL (AVL *, bool *, char);
AVL *rightRotation_AVL (AVL *, bool *, char);
AVL *insert_AVL (AVL *, int, bool *);
AVL *search_AVL (int);
AVL *delete_AVL (AVL *, int, bool *, bool *);

RBTree *create_RB (int data);
RBTree *leftRotation_RB (RBTree *, bool *, bool *, char);
RBTree *rightRotation_RB (RBTree *, bool *, bool *, char);
RBTree *insert_RB (RBTree *, int data, bool *, bool *);
RBTree *delete_RB (RBTree *, int data, bool *, bool *, bool *);
short int search_RB (int data);

RBTree *RootRB = NULL;
AVL *RootAVL = NULL;
int replaceNum = -1;

int main () {
	char tree[11], op[10];
	scanf("%s", tree);

	if (!strcmp(tree, "red_black")) {
		while (1) {
			int data;
			scanf("%s", op);
			
			if (!strcmp(op, "insert")) {
				scanf("%d", &data);
				bool unbalance = true, changeParentColor = false;
				insert_RB(RootRB, data, &unbalance, &changeParentColor);
			}
			else if (!strcmp(op, "search")) {
				scanf("%d", &data);
				short int color = search_RB(data);
				if (color == 0) {
					printf("black\n");
				}
				else if (color == 1) {
					printf("red\n");
				}
				else {
					printf("Not found\n");
				}
			}
			else if (!strcmp(op, "delete")) {
				scanf("%d", &data);
				bool unbalance = true, find = false, changeParentColor = false;
				delete_RB(RootRB, data, &unbalance, &find, &changeParentColor);
				replaceNum = -1;
			}
			else if (!strcmp(op, "exist")) {
				scanf("%d", &data);
				short int color = search_RB(data);
				if (color != -1) {
					printf("exist\n");
				}
				else {
					printf("Not exist\n");
				}
			}
			else {
				break;
			}
		}
	}
	else {
		while (1) {
			int data;
			scanf("%s", op);

			if (!strcmp(op, "insert")) {
				scanf("%d", &data);
				bool unbalance = true;
				insert_AVL(RootAVL, data, &unbalance);
			}
			else if (!strcmp(op, "search")) {
				scanf("%d", &data);
				AVL *target = search_AVL(data);
				if (target != NULL) {
					printf("%d\n", target->bf);
				}
				else {
					printf("Not found\n");
				}
			}
			else if (!strcmp(op, "delete")) {
				scanf("%d", &data);
				bool unbalance = true, find = false;
				delete_AVL(RootAVL, data, &unbalance, &find);
				replaceNum = -1;
			}
			else if (!strcmp(op, "exist")) {
				scanf("%d", &data);
				AVL *target = search_AVL(data);
				if (target != NULL) {
					printf("exist\n");
				}
				else {
					printf("Not exist\n");
				}
			}
			else {
				break;
			}
		}
	}

	return 0;
}

AVL *create_AVL (int data) {
	AVL *node = malloc (sizeof(AVL));
	node->rightChild = node->leftChild = NULL;
	node->data = data;
	node->bf = 0;
	return node;
}

AVL *leftRotation_AVL (AVL *parent, bool *unbalanced, char op) {
	AVL *grandChild, *child = parent->leftChild;

	if (child->bf == 1  || (child->bf == 0 && op == 'd') ) {
		parent->leftChild = child->rightChild;
		child->rightChild = parent;
		if (op == 'i') {
			// LL Rotation
			child->bf = parent->bf = 0;
			*unbalanced = false;
		}
		else {
			switch (child->bf) {
				// R0 Rotation
				case 0:
					parent->bf = 1;
					child->bf = -1;
					*unbalanced = false;
					break;
			 	// R1 Rotation
				case 1:
					child->bf = parent->bf = 0;
					break;
			}
		}

		if (RootAVL == parent) {
			RootAVL = child;
		}

		return child;
	}
	else {
		// LR && R-1 Rotation
		if (op == 'i') {
			*unbalanced = false;
		}
		grandChild = child->rightChild;
		child->rightChild = grandChild->leftChild;
		parent->leftChild = grandChild->rightChild;
		grandChild->leftChild = child;
		grandChild->rightChild = parent;
		switch (grandChild->bf) {
			case 0:
				grandChild->bf = child->bf = parent->bf = 0;
				break;
			case 1:
				grandChild->bf = child->bf = 0;
				parent->bf = -1;
				break;
			case -1:
				grandChild->bf = parent->bf = 0;
				child->bf = 1;
				break;
		}

		if (RootAVL == parent) {
			RootAVL = grandChild;
		}

		return grandChild;
	}
}

AVL *rightRotation_AVL (AVL *parent, bool *unbalanced, char op) {
	AVL *grandChild, *child = parent->rightChild;
	
	if (child->bf == -1 || (child->bf == 0 && op == 'd') ) {
		parent->rightChild = child->leftChild;
		child->leftChild = parent;
		if (op == 'i') {
			// RR Rotation
			parent->bf = child->bf = 0;
			*unbalanced = false;
		}
		else {
			switch (child->bf) {
				case 0:
					// L0 Rotation
					parent->bf = -1;
					child->bf = 1;
					*unbalanced = false;
					break;
				case -1:
					// L-1 Rotation
					parent->bf = child->bf = 0;
					break;
			}
		}

		if (RootAVL == parent) {
			RootAVL = child;
		}

		return child;
	}
	else {
		// RL & L1 Rotation
		if (op == 'i') {
			*unbalanced = false;
		}
		grandChild = child->leftChild;
		child->leftChild = grandChild->rightChild;
		parent->rightChild = grandChild->leftChild;
		grandChild->leftChild = parent;
		grandChild->rightChild = child;
		switch (grandChild->bf) {
			case 0:
				grandChild->bf = child->bf = parent->bf = 0;
				break;
			case 1:
				grandChild->bf = parent->bf = 0;
				child->bf = -1;
				break;
			case -1:
				grandChild->bf = child->bf = 0;
				parent->bf = 1;
				break;
		}

		if (RootAVL == parent) {
			RootAVL = grandChild;
		}

		return grandChild;
	}
}

AVL *insert_AVL (AVL *target, int data, bool *unbalance) {
	if (!RootAVL) {
		AVL *new = create_AVL(data);
		RootAVL = new;
	}
	else if (!target) {
		return create_AVL(data);
	}
	else if (data > target->data) {
		AVL *new = insert_AVL(target->rightChild, data, unbalance);
		if (new) {
			target->rightChild = new;
		}
		if (*unbalance) {
			switch (target->bf) {
				case -1:
					return rightRotation_AVL(target, unbalance, 'i');
				case 0:
					target->bf = -1;
					break;
				case 1:
					target->bf = 0;
					*unbalance = false;
					break;
			}
		}
	}
	else if (data == target->data) {
		*unbalance = false;
	}
	else if (data < target->data) {
		AVL *new = insert_AVL(target->leftChild, data, unbalance);
		if (new) {
			target->leftChild = new;
		}
		if (*unbalance) {
			switch (target->bf) {
				case -1:
					target->bf = 0;
					*unbalance = false;
					break;
				case 0:
					target->bf = 1;
					break;
				case 1:
					return leftRotation_AVL(target, unbalance, 'i');
			}
		}
	}
	return NULL;
}

AVL *delete_AVL (AVL *target, int data, bool *unbalance, bool *find) {
	if (target) {
		if (!*find) {
			// finding delete data
			if (data > target->data) {
				AVL *new = delete_AVL(target->rightChild, data, unbalance, find);
				if (target->rightChild && target->rightChild->data == data) {
					free(target->rightChild);
					target->rightChild = new;
				}
				else if (new) {
					target->rightChild = new;
				}
				if (*unbalance) {
					switch (target->bf) {
						case -1:
							target->bf = 0;
							break;
						case 0:
							target->bf = 1;
							*unbalance = false;
							break;
						case 1:
							return leftRotation_AVL(target, unbalance, 'd');
					}
				}
			}
			else if (data == target->data) {
				*find = true;
				if (!target->leftChild && !target->rightChild) {
					// delete from a leaf
					return NULL;
				}
				else if (target->leftChild) {
					// delete from internal node and use biggest leftChild to replace
					AVL *new = delete_AVL(target->leftChild, data, unbalance, find);
					target->data = replaceNum;
					if (target->leftChild && target->leftChild->data == replaceNum) {
						free(target->leftChild);
						target->leftChild = new;
					}
					if (new) {
						target->leftChild = new;
					}
					if (*unbalance) {
						switch (target->bf) {
							case -1:
								return rightRotation_AVL(target, unbalance, 'd');
							case 0:
								target->bf = -1;
								*unbalance = false;
								break;
							case 1:
								target->bf = 0;
								break;
						}
					}
				}
				else if (target->rightChild) {
					// delete from internal node and use rightChild to replace
					target->data = target->rightChild->data;
					target->rightChild = NULL;
					target->bf = 0;
				}
			}
			else if (data < target->data) {
				AVL *new = delete_AVL(target->leftChild, data, unbalance, find);
				if (target->leftChild && target->leftChild->data == data) {
					free(target->leftChild);
					target->leftChild = new;
				}
				if (new) {
					target->leftChild = new;
				}
				if (*unbalance) {
					switch (target->bf) {
						case -1:
							return rightRotation_AVL(target, unbalance, 'd');
						case 0:
							target->bf = -1;
							*unbalance = false;
							break;
						case 1:
							target->bf = 0;
							break;
					}
				}
			}
		}
		else {
			if (target->rightChild) {
				// finding biggest leftChild to replace
				AVL *new = delete_AVL(target->rightChild, data, unbalance, find);
				if (target->rightChild && target->rightChild->data == replaceNum) {
					free(target->rightChild);
					target->rightChild = new;
				}
				if (new) {
					target->rightChild = new;
				}
				if (*unbalance) {
					switch (target->bf) {
						case -1:
							target->bf = 0;
							break;
						case 0:
							target->bf = 1;
							*unbalance = false;
							break;
						case 1:
							return leftRotation_AVL(target, unbalance, 'd');
					}
				}
			}
			else {
				replaceNum = target->data;
				return target->leftChild;
			}
		}
	}
	else {
		*unbalance = false;
	}
	return NULL;
}

AVL *search_AVL (int data) {
	AVL *target = RootAVL;

	while (target) {
		if (target->data == data) {
			break;
		}
		else if (data > target->data) {
			target = target->rightChild;
		}
		else {
			target = target->leftChild;
		}
	}

	return target;
}

RBTree *create_RB (int data) {
	RBTree *new = calloc (1, sizeof(RBTree));
	new->data = data;
       	return new;	
}

RBTree *leftRotation_RB (RBTree *parent, bool *unbalanced, bool *changeParentColor, char op) {
	RBTree *grandChild, *child = parent->leftChild;

	if (child->leftColor == red && (parent->rightColor == black || op == 'd') ) {
		// LL Rotation
		*unbalanced = false;
		parent->leftChild = child->rightChild;
		child->rightChild = parent;
		if (op == 'i') {
			child->leftColor = child->rightColor = red;
			parent->leftColor = black;
		}
		else {
			parent->leftColor = child->rightColor;
			child->leftColor = child->rightColor = black;
		}

		if (RootRB == parent) {
			RootRB = child;
		}

		return child;
	}
	else if (child->rightColor == red && (parent->rightColor == black || op == 'd') ) {
		// LR Rotation
		*unbalanced = false;
		grandChild = child->rightChild;
		child->rightChild = grandChild->leftChild;
		parent->leftChild = grandChild->rightChild;
		grandChild->leftChild = child;
		grandChild->rightChild = parent;
		if (op == 'i') {
			parent->leftColor = child->rightColor = black;
			grandChild->rightColor = grandChild->leftColor = red;
		}
		else {
			parent->leftColor = grandChild->rightColor;
			child->rightColor = grandChild->rightColor = grandChild->leftColor = black;
		}
		
		if (RootRB == parent) {
			RootRB = grandChild;
		}

		return grandChild;
	}
	else if ( (parent->rightColor == red && op == 'i') || (parent->leftColor == red && op == 'd') ) {
		// LRR && LLR Rotation
		if (op == 'i') {
			parent->leftColor = parent->rightColor = black;
			*changeParentColor = true;
		}
		else {
			*unbalanced = false;
			grandChild = child->rightChild;
			if (grandChild->leftColor == red) {
				parent->leftChild = grandChild->rightChild;
				child->rightChild = grandChild->leftChild;
				grandChild->leftChild = child;
				grandChild->rightChild = parent;
				parent->leftColor = grandChild->rightColor;
				grandChild->rightColor = black;
				if (RootRB == parent) {
					RootRB = grandChild;
				}
				return grandChild;
			}
			else if (grandChild->rightColor == red) {
				RBTree *newParent = grandChild->rightChild;
				parent->leftChild = grandChild->rightChild->rightChild;
				grandChild->rightChild = grandChild->rightChild->leftChild;
				newParent->leftChild = child;
				newParent->rightChild = parent;
				parent->leftColor = grandChild->rightColor = black;
				newParent->leftColor = red;
				if (RootRB == parent) {
					RootRB = newParent;
				}
				return newParent;
			}
			else {
				parent->leftChild = grandChild;
				child->rightChild = parent;
				if (RootRB == parent) {
					RootRB = child;
				}
				return child;

			}
		}
	}
	else if (op == 'd') {
		parent->leftColor = red;
		*changeParentColor = true;
	}
	return NULL;
}

RBTree *rightRotation_RB (RBTree *parent, bool *unbalanced, bool *changeParentColor, char op) {
	RBTree *grandChild, *child = parent->rightChild;

	if (child->rightColor == red && (parent->leftColor == black || op == 'd') ) {
		// RR Rotation
		*unbalanced = false;
		parent->rightChild = child->leftChild;
		child->leftChild = parent;
		if (op == 'i') {
			parent->rightColor = black;
			child->leftColor = red;
		}
		else {
			parent->rightColor = child->leftColor;
			child->rightColor = child->leftColor = black;
		}

		if (RootRB == parent) {
			RootRB = child;
		}

		return child;
	}
	else if (child->leftColor == red && (parent->leftColor == black || op == 'd') ){
		// RL Rotation
		*unbalanced = false;
		grandChild = child->leftChild;
		parent->rightChild = grandChild->leftChild;
		child->leftChild = grandChild->rightChild;
		grandChild->leftChild = parent;
		grandChild->rightChild = child;
		if (op == 'i') {
			parent->rightColor = child->leftColor = black;
			grandChild->leftColor = grandChild->rightColor = red;
		}
		else {
			parent->rightColor = grandChild->leftColor;
			child->leftColor = grandChild->leftColor = grandChild->rightColor = black;
		}

		if (RootRB == parent) {
			RootRB = grandChild;
		}

		return grandChild;
	}
	else if ( (parent->leftColor == red && op == 'i') || (parent->rightColor == red && op == 'd') ) {
		// RLL & RRL Rotation
		if (op == 'i') {
			parent->leftColor = parent->rightColor = black;
			*changeParentColor = true;
		}
		else {
			*unbalanced = false;
			grandChild = child->leftChild;
			if (grandChild->leftColor == red) {
				RBTree *newParent = grandChild->leftChild;
				parent->rightChild = grandChild->leftChild->leftChild;
				grandChild->leftChild = grandChild->leftChild->rightChild;
				newParent->leftChild = parent;
				newParent->rightChild = child;
				grandChild->leftColor = parent->rightColor = black;
				newParent->rightColor = red;
				if (RootRB == parent) {
					RootRB = grandChild;
				}
				return newParent;
			}
			else if (grandChild->rightColor == red) {
				parent->rightChild = grandChild->leftChild;
				child->leftChild = grandChild->rightChild;
				grandChild->leftChild = parent;
				grandChild->rightChild = child;
				parent->rightColor = grandChild->leftColor;
				grandChild->leftColor = black;
				if (RootRB == parent) {
					RootRB = grandChild;
				}
				return grandChild;
			}
			else {
				parent->rightChild = grandChild;
				child->leftChild = parent;
				if (RootRB == parent) {
					RootRB = child;
				}
				return child;
			}
		}
	}
	else if (op == 'd') {
		parent->rightColor = red;
		*changeParentColor = true;
	}
	return NULL;
}

RBTree *insert_RB (RBTree *target, int data, bool *unbalance, bool *changeParentColor) {
	if (!RootRB) {
		RBTree *new = create_RB(data);
		RootRB = new;
	}
	else if (!target) {
		*changeParentColor = true;
		return create_RB(data);
	}
	else if (data > target->data) {
		RBTree *new = insert_RB(target->rightChild, data, unbalance, changeParentColor);
		if (new) {
			target->rightChild = new;
		}

		if (*changeParentColor) {
			target->rightColor = red;
			*changeParentColor = false;
		}
		else if (*unbalance) {
			switch (target->rightColor) {
				case black:
					*unbalance = false;
					break;
				case red:
					return rightRotation_RB(target, unbalance, changeParentColor, 'i');
			}
		}
	}
	else if (data == target->data) {
		*unbalance = false;
	}
	else if (data < target->data) {
		RBTree *new = insert_RB(target->leftChild, data, unbalance, changeParentColor);
		if (new) {
			target->leftChild = new;
		}

		if (*changeParentColor) {
			target->leftColor = red;
			*changeParentColor = false;
		}
		else if (*unbalance) {
			switch (target->leftColor) {
				case black:
					*unbalance = false;
					break;
				case red:
					return leftRotation_RB(target, unbalance, changeParentColor, 'i');
			}
		}
	}
	return NULL;
}

RBTree *delete_RB (RBTree *target, int data, bool *unbalance, bool *find, bool *changeParentColor) {
	if (target) {
		if (!*find) {
			// finding delete data
			if (data > target->data) {
				RBTree *new = delete_RB(target->rightChild, data, unbalance, find, changeParentColor);
				if (target->rightChild && target->rightChild->data == data) {
					// free(target->rightChild);
					target->rightChild = new;
					if (target->rightColor == red) {
						target->rightColor = black;
						*unbalance = false;
					}
				}
				else if (new) {
					target->rightChild = new;
				}
				
				if (*changeParentColor && target->rightColor == red) {
					target->rightColor = black;
					*changeParentColor = *unbalance = false;
				}
				*changeParentColor = false;

				if (*unbalance) {
					return leftRotation_RB(target, unbalance, changeParentColor, 'd');
				}
			}
			else if (data == target->data) {
				*find = true;
				if (!target->leftChild && !target->rightChild) {
					// delete from a leaf
					if (target == RootRB) {
						RootRB = NULL;
					}
				}
				else if (target->leftChild) {
					// delete from internal node and use biggest leftChild to replace
					RBTree *new = delete_RB(target->leftChild, data, unbalance, find, changeParentColor);
					target->data = replaceNum;
					if (target->leftChild && target->leftChild->data == replaceNum) {
						// free(target->leftChild);
						target->leftChild = new;
						if (target->leftColor == red) {
							target->leftColor = black;
							*unbalance = false;
						}
					}
					if (new) {
						target->leftChild = new;
					}
					
					if (*changeParentColor && target->leftColor == red) {
						target->leftColor = black;
						*changeParentColor = *unbalance = false;
					}
					*changeParentColor = false;

					if (*unbalance) {
						return rightRotation_RB(target, unbalance, changeParentColor, 'd');
					}
				}
				else {
					// delete from internal node and use rightChild to replace
					*unbalance = false;
					target->data = target->rightChild->data;
					target->rightColor = black;
					target->rightChild = NULL;
				}
			}
			else if (data < target->data) {
				RBTree *new = delete_RB(target->leftChild, data, unbalance, find, changeParentColor);
				if (target->leftChild && target->leftChild->data == data) {
					// free(target->leftChild);
					target->leftChild = new;
					if (target->leftColor == red) {
						target->leftColor = black;
						*unbalance = false;
					}
				}
				if (new) {
					target->leftChild = new;
				}

				if (*changeParentColor && target->leftColor == red) {
					target->leftColor = black;
					*changeParentColor = *unbalance = false;
				}
				*changeParentColor = false;

				if (*unbalance) {
					return rightRotation_RB(target, unbalance, changeParentColor, 'd');
				}
			}
		}
		else {
			if (target->rightChild) {
				// finding biggest leftChild to replace
				RBTree *new = delete_RB(target->rightChild, data, unbalance, find, changeParentColor);
				if (target->rightChild && target->rightChild->data == replaceNum) {
					// free(target->rightChild);
					target->rightChild = new;
					if (target->rightColor == red) {
						target->rightColor = black;
						*unbalance = false;
					}
				}
				if (new) {
					target->rightChild = new;
				}

				if (*changeParentColor && target->rightColor == red) {
					target->rightColor = black;
					*changeParentColor = *unbalance = false;
				}
				*changeParentColor = false;

				if (*unbalance) {
					return leftRotation_RB(target, unbalance, changeParentColor, 'd');
				}
			}
			else {
				replaceNum = target->data;
				if (target->leftChild) {
					*unbalance = false;
					target->data = target->leftChild->data;
					target->leftColor = black;
					target->leftChild = NULL;
				}
			}
		}
	}
	else {
		*unbalance = false;
	}
	return NULL;
}

short int search_RB (int data) {
	RBTree *target = RootRB;
	short int color = -1;
	if (RootRB && data == RootRB->data) {
		return black;
	}
	while (1) {
		if (!target) {
			return -1;
		}
		else if (data > target->data) {
			color = target->rightColor;
			target = target->rightChild;
		}
		else if (data == target->data) {
			return color;
		}
		else {
			color = target->leftColor;
			target = target->leftChild;
		}
	}
}
