#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	int data;
	struct node *leftchild;
	struct node *rightchild;
}node;

typedef struct tree {
	node *root;
	char *s;
	struct tree *next; 
}tree;

node *PreInToTree (int *, int *, int);
node *PostInToTree (int *, int *, int);
void preorder (node *);
void postorder (node *);

int main () {
	FILE *fpr = fopen("input_01.txt", "r");
	tree *first = malloc (sizeof(tree));
	first = NULL;
	tree *last = malloc (sizeof(tree));
	int testDataNum, nodeNum; 
	char *orderKind = malloc (sizeof(char) * 20);
       	fscanf(fpr, "%d", &testDataNum);
	for (int i = 0; i < testDataNum; i++) {
		fscanf(fpr, "%s", orderKind);
		fscanf(fpr, "%d", &nodeNum);
		if (!strcmp("preorder-inorder", orderKind)) {
			int *preList = malloc (sizeof(int) * nodeNum);
			int *inList = malloc (sizeof(int) * nodeNum);
			for (int j = 0; j < nodeNum; j++) {
				fscanf(fpr, "%d", preList+j);
			}
			for (int j = 0; j < nodeNum; j++) {
				fscanf(fpr, "%d", inList+j);
			}
			tree *new = malloc (sizeof(tree));
			new->next = NULL;
			new->root = PreInToTree(preList, inList, nodeNum);
			new->s = malloc (sizeof(char) * 20);
			strcpy(new->s, "preorder-inorder");
			if (first == NULL) {
				first = new;
				last = new;
			}
			else {
				last->next = new;
				last = new;
			}
		}
		else{
			int *postList = malloc (sizeof(int) * nodeNum);
			int *inList = malloc (sizeof(int) * nodeNum);
			for (int j = 0; j < nodeNum; j++) {
				fscanf(fpr, "%d", postList+j);
			}
			for (int j = 0; j < nodeNum; j++) {
				fscanf(fpr, "%d", inList+j);
			}
			tree *new = malloc (sizeof(tree));
			new->next = NULL;
			new->root = PostInToTree(postList, inList, nodeNum);
			new->s = malloc (sizeof(char) * 20);
			strcpy(new->s, "postorder-inorder");
			if (first == NULL) {
				first = new;
				last = new;
			}
			else {
				last->next = new;
				last = new;
			}
		}

	}

	while (first != NULL) {
		if (!strcmp("preorder-inorder", first->s)) {
			postorder(first->root);
		}
		else {
			preorder(first->root);
		}
		first = first->next;
		printf("\n");
	}

	return 0;
}

node *PreInToTree (int *preList, int *inList, int n) {
	int rootPos = -1; // inoreder root position
	node *root = malloc (sizeof(node));
	root->data = preList[0];
	if (n > 1) {
		int *leftPreList = malloc(sizeof(int));
		int *leftInList = malloc(sizeof(int));
		int *rightPreList = malloc(sizeof(int));
		int *rightInList = malloc(sizeof(int));
		for (int i = 0; i < n; i++) {
			if (inList[i] == preList[0]) {
				rootPos = i;
				continue;
			}
			if (rootPos < 0) { // not found rootPos
				leftPreList[i] = preList[i+1];
				leftInList[i] = inList[i];
				leftPreList = realloc(leftPreList, sizeof(int) * (i + 2));
				leftInList = realloc(leftInList, sizeof(int) * (i + 2));
			}
			else {
				rightPreList[i-rootPos-1] = preList[i];
				rightInList[i-rootPos-1] = inList[i];
				rightPreList = realloc(rightPreList, sizeof(int) * (i - rootPos + 1));
				rightInList = realloc(rightInList, sizeof(int) * (i - rootPos + 1));
			}
		}

		if (rootPos == 0) {
			root->leftchild = NULL;
			root->rightchild = PreInToTree(rightPreList, rightInList, (n-rootPos-1));
		}
		else if (rootPos == n-1) {
			root->leftchild = PreInToTree(leftPreList, leftInList, rootPos);
			root->rightchild = NULL;
		}
		else{
			root->leftchild = PreInToTree(leftPreList, leftInList, rootPos);
			root->rightchild = PreInToTree(rightPreList, rightInList, (n-rootPos-1));
		}
		free(leftPreList);
		free(leftInList);
		free(rightPreList);
		free(rightInList);
	}
	else {
		root->leftchild = NULL;
		root->rightchild = NULL;
	}
	return root;
}


node *PostInToTree (int *postList, int *inList, int n) {
	int rootPos = -1; // inoreder root position
	node *root = malloc (sizeof(node));
	root->data = postList[n-1];
	if (n > 1) {
		int *leftPostList = malloc(sizeof(int));
		int *leftInList = malloc(sizeof(int));
		int *rightPostList = malloc(sizeof(int));
		int *rightInList = malloc(sizeof(int));
		for (int i = 0; i < n; i++) {
			if (inList[i] == postList[n-1]) {
				rootPos = i;
				continue;
			}
			if (rootPos < 0) { // not found rootPos
				leftPostList[i] = postList[i];
				leftInList[i] = inList[i];
				leftPostList = realloc(leftPostList, sizeof(int) * (i + 2));
				leftInList = realloc(leftInList, sizeof(int) * (i + 2));
			}
			else {
				rightPostList[i-rootPos-1] = postList[i-1];
				rightInList[i-rootPos-1] = inList[i];
				rightPostList = realloc(rightPostList, sizeof(int) * (i - rootPos + 1));
				rightInList = realloc(rightInList, sizeof(int) * (i - rootPos + 1));
			}
		}

		if (rootPos == 0) {
			root->leftchild = NULL;
			root->rightchild = PostInToTree(rightPostList, rightInList, (n-rootPos-1));
		}
		else if (rootPos == n-1) {
			root->leftchild = PostInToTree(leftPostList, leftInList, rootPos);
			root->rightchild = NULL;
		}
		else{
			root->leftchild = PostInToTree(leftPostList, leftInList, rootPos);
			root->rightchild = PostInToTree(rightPostList, rightInList, (n-rootPos-1));
		}
		free(leftPostList);
		free(leftInList);
		free(rightPostList);
		free(rightInList);
	}
	else {
		root->leftchild = NULL;
		root->rightchild = NULL;
	}
	return root;
}

void preorder (node *root) {
	if (root != NULL) {
		printf("%d ", root->data);
		preorder(root->leftchild);
		preorder(root->rightchild);
	}
}

void postorder (node *root) {
	if (root != NULL) {
		postorder(root->leftchild);
		postorder(root->rightchild);
		printf("%d ", root->data);
	}

}
