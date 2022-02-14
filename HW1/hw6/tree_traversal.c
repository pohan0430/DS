#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define MAXLENGTH 100
#define bool int

typedef struct node {
	struct node *leftnode;
	char key;
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
		s1->top++;
		s1->s[s1->top] = item;
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

node* PostfixToTree(char *);
node* PrefixToTree(char *);

void preorder (node *);
void postorder (node *);

int main () {
	int i = 0, j = 2, prefixLength = MAXLENGTH, postfixLength = MAXLENGTH;
	char *prefix = malloc (prefixLength * sizeof(char));
	char *postfix = malloc (postfixLength * sizeof(char));

	scanf("%s", prefix);
	scanf("%s", postfix);

	node *postRoot = PostfixToTree(postfix);
	node *preRoot = PrefixToTree(prefix);

	postorder(preRoot);
	printf("\n");
	preorder(postRoot);
	printf("\n");

	return 0;
}

node *PostfixToTree(char *postfix) {
	Stack s = create_stack(strlen(postfix));
	node *root;
	for (int i = 0; i < strlen(postfix); i++) {
		root = create_node(postfix[i]);
		if(postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/' || postfix[i] == '^') {
			root->rightnode = pop(&s);
			root->leftnode = pop(&s);
		}
		else {
			root->rightnode = NULL;
			root->leftnode = NULL;
		}
		push(&s, root);
	}
	return root;
}

node *PrefixToTree(char *prefix) {
	Stack s = create_stack(strlen(prefix));
	node *root;
	for (int i = strlen(prefix)-1; i >= 0; i--) {
		root = create_node(prefix[i]);
		if(prefix[i] == '+' || prefix[i] == '-' || prefix[i] == '*' || prefix[i] == '/' || prefix[i] == '^') {
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

void preorder (node *root) {
	if (root != NULL) {
		printf("%c", root->key);
		preorder(root->leftnode);
		preorder(root->rightnode);
	}
}

void postorder (node *root) {
	if (root != NULL) {
		postorder(root->leftnode);
		postorder(root->rightnode);
		printf("%c", root->key);
	}
}
