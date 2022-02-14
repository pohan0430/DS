#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define MAXLENGTH 15
#define bool int

typedef struct node {
	struct node *leftnode;
	int key;
	struct node *rightnode;
}node;

typedef struct {
	node **s;
	int size;
	int top;
}Stack;

Stack create_stack (int n) {
	Stack s1;
	s1.size = n;
	s1.top = -1;
	s1.s = malloc(s1.size * sizeof(node *));
	return s1;
}

bool isFull (Stack s1) {
	return s1.top == s1.size-1;
}

bool isEmpty (Stack s1) {
	return s1.top == -1;
}

bool push(Stack *s1, node *item) {
	if (isFull(*s1)) {
		return false;
	}
	else {
		s1->s[++s1->top] = item;
		return true;
	}
}

node* pop (Stack *s1) {
	if (isEmpty(*s1)) {
		node *null = NULL;
		return null;
	}
	else {
		return s1->s[s1->top--];
	}
}

node* create_node (char c) {
	node *n = malloc(sizeof(node));
	n->key = c;
	n->leftnode = NULL;
	n->rightnode = NULL;
	return n;
}

node* SpecialPreorderToTree(int *, int);
node* swapTree(node *);

void preorder (node *);
void inorder (node *);

int main () {
	int n; // how many numbers
	scanf("%d", &n);
	int *spList = malloc (sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		scanf("%d", spList+i);
	}
	
	node *TreeRoot = SpecialPreorderToTree(spList, n);
	TreeRoot = swapTree(TreeRoot);
	
	preorder(TreeRoot);
	printf("\n");
	inorder(TreeRoot);
	printf("\n");

	return 0;
}

node *SpecialPreorderToTree(int *spList, int n) {
	Stack s = create_stack(n);
	node *root;
	for (int i = n-1; i >= 0; i--) {
		root = create_node(spList[i]);
		if(spList[i] != -1) {
			root->leftnode = pop(&s);
			root->rightnode = pop(&s);
		}
		else {
			root->rightnode = NULL;
			root->leftnode = NULL;
		}
		push(&s, root);
	}
	return root;
}

node *swapTree(node *root) {
	if (root->leftnode->key != -1) {
		root->leftnode = swapTree(root->leftnode);
	}
	if (root->rightnode->key != -1) {
		root->rightnode = swapTree(root->rightnode);
	}
	if (!(root->leftnode == NULL && root->rightnode == NULL)) {
		node *temp = root->leftnode;
		root->leftnode = root->rightnode;
		root->rightnode = temp;
		return root;
	}	
}

void preorder (node *root) {
	if (root->key != -1) {
		printf("%d ", root->key);
		preorder(root->leftnode);
		preorder(root->rightnode);
	}
}

void inorder (node *root) {
	if (root->key != -1) {
		inorder(root->leftnode);
		printf("%d ", root->key);
		inorder(root->rightnode);
	}
}
