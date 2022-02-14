#include <stdio.h>
#include <stdlib.h>

typedef struct leftistTree {
	struct leftistTree *leftchild;
	struct leftistTree *rightchild;
	int data;
	int shortest;
}leftistTree;

leftistTree *create_leftistTree (int *, int );
leftistTree *minUnion (leftistTree *, leftistTree *);
leftistTree *insert (leftistTree *, int);
leftistTree *delete (leftistTree *);
void preorder (leftistTree *);
void inorder (leftistTree *);
void postorder (leftistTree *);

int main () {
	int array[] = {26, 5, 2, 80, 49, 13, 1};

	leftistTree *root = create_leftistTree(array, 7);
	
	preorder(root);
	printf("\n");
	inorder(root);
	printf("\n");
	postorder(root);
	printf("\n");
	
	root = insert(root, 10);
	
	preorder(root);
	printf("\n");
	inorder(root);
	printf("\n");
	postorder(root);
	printf("\n");

	root = insert(root, 8);

	preorder(root);
	printf("\n");
	inorder(root);
	printf("\n");
	postorder(root);
	printf("\n");

	root = delete(root);

	preorder(root);
	printf("\n");
	inorder(root);
	printf("\n");
	postorder(root);
	printf("\n");

	return 0;
}

leftistTree *create_leftistTree (int *array, int size) {
	leftistTree **list = malloc (sizeof(leftistTree *) * size);
	for (int i = 0; i < size; i++) {
		list[i] = malloc (sizeof(leftistTree));
		list[i]->data = array[i];
		list[i]->leftchild = NULL;
		list[i]->rightchild = NULL;
		list[i]->shortest = 1;
	}
	
	for (int i = 1; i < size; i++) {
		list[0] = minUnion(list[0], list[i]);
	}
	
	leftistTree *root = list[0];
	free(list);

	return 	root;
}

leftistTree *minUnion (leftistTree *first, leftistTree *second) {
	if (first != NULL && second != NULL) {
		leftistTree *root = first->data <= second->data ? first : second;

		// let first be the smallest key
		if (first->data > second->data) { 
			leftistTree *temp = first;
			first = second;
			second = temp;
		}

		if (first->leftchild == NULL) {
		       root->leftchild = second;
	      	}
		else {		
			first = first->rightchild;
			root->rightchild = minUnion(first, second);
			if (root->leftchild->shortest < root->rightchild->shortest) {
				leftistTree *temp = root->leftchild;
				root->leftchild = root->rightchild;
				root->rightchild = temp;
			}
			root->shortest = root->rightchild->shortest > root->leftchild->shortest ? root->leftchild->shortest + 1 : root->rightchild->shortest + 1;
		}

		return root;
	}
	else if (first == NULL) {
		return second;
	}
	else {
		return first;
	}
}

leftistTree *insert (leftistTree *root, int data) {
	leftistTree *second = malloc (sizeof(leftistTree));
	second->leftchild = NULL;
	second->rightchild = NULL;
	second->data = data;
	second->shortest = 1;

	root = minUnion(root, second);

	return root;
}

leftistTree *delete (leftistTree *root) {
	root = minUnion(root->leftchild, root->rightchild);

	return root;
}

void preorder (leftistTree *root) {
	if (root != NULL) {
		printf("%d ", root->data);
		preorder(root->leftchild);
		preorder(root->rightchild);
	}
}

void inorder (leftistTree *root) {
	if (root != NULL) {
		inorder(root->leftchild);
		printf("%d ", root->data);
		inorder(root->rightchild);
	}
}

void postorder (leftistTree *root) {
	if (root != NULL) {
		postorder(root->leftchild);
		postorder(root->rightchild);
		printf("%d ", root->data);
	}
}
