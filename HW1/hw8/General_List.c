#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FunctionLength 10
#define bool int
#define false 0
#define true 1
#define MAXSTRINGLENGTH 100

typedef struct node {
	char data;
	struct node *next, *down;
}GLnode;

typedef struct tree {
	GLnode *root;
	int treeId;
}tree;

typedef struct treenode {
	tree *tree;
	struct treenode *next;
}Tnode;
	       
tree *read ();
GLnode *read_node(char *, GLnode *, int);
tree *copy (Tnode *);
GLnode *copy_tree (GLnode *); 
bool isequal(Tnode *);
bool isequal_tree(GLnode *, GLnode *);
Tnode *clear (Tnode *);
char *write (Tnode *);
void write_node (GLnode *, char *);

int i = 1; // read counting char

int main () {
	int n; // represent amount of commands
	char *functions = malloc (sizeof(char) * FunctionLength);
	char c;
	Tnode *head = NULL;
	scanf("%d", &n); 
	
	for (int j = 0; j < n; j++) {
		scanf("%s", functions);
		if (!strcmp("read", functions)) {
			tree *newTree = malloc(sizeof(tree));
			newTree = read();
			Tnode *newTnode = malloc (sizeof(Tnode));
			newTnode->tree = newTree;
			if (head == NULL) {
				newTnode->next = NULL;
				head = newTnode;
			}
			else {
				newTnode->next = head;
				head = newTnode;
			}
			i = 1;
		}
		else if (!strcmp("copy", functions)) {
			tree *newTree = copy(head);
			Tnode *newTnode = malloc (sizeof(Tnode));
			newTnode->tree = newTree;
			newTnode->next = head;
			head = newTnode;
			i = 1;
		}
		else if (!strcmp("isequal", functions)) {
			bool equal = isequal(head);
			if (equal) {
				printf("true\n");
			}
			else {
				printf("false\n");
			}
		}
		else if (!strcmp("clear", functions)) {
			head = clear(head);
			printf("cleared\n");
		}
		else {
			printf("%s\n", write(head));
		}
	}

	return 0;
}

tree *read () {
	int treeId;
	char *str = malloc (sizeof(char) * MAXSTRINGLENGTH);

	scanf("%d %s", &treeId, str);

	tree *t = malloc(sizeof(tree));
	t->treeId = treeId;
	t->root = malloc (sizeof(GLnode));
	t->root->down = NULL;
	t->root->down = read_node(str, t->root, strlen(str));

	printf("readed\n"); 
	return t;
}

GLnode *read_node (char *str, GLnode *root, int len) {
	GLnode *q = malloc (sizeof(GLnode));
	while (true) {
		if (str[i] == ',') {
			i++;
		}
		GLnode *new = malloc (sizeof(GLnode));
		q = root->down;
		new->data = str[i];
		new->next = NULL;
		if (q == NULL) {
			root->down = new;
		}
		else {
			GLnode *p = malloc (sizeof(GLnode));
			int flag = false;
			while (q != NULL && q->data < new->data) {
				flag = true;
				p = q;
				q = q->next;
			}
			if (flag && q == NULL) {
				p->next = new;
			}
			else if (flag && q->data > new->data) {
				new->next = q;
				p->next = new;
			}
			else {
				new->next = root->down;
				root->down = new;
			}			
		}
		switch (str[++i]) {
			case '(' :
				i++;
				new->down = read_node(str, new, len);
				break;
			case ',':
				i++;
				new->down = NULL;
				break;
			case ')' :
				i++;
				return root->down;
		}
		if (str[i] == ')' && str[i+1] == ')') {
			i++;
			return root->down;
		}
		if (i == len - 1) {
			return root->down;
		}
	}
}

tree *copy (Tnode *head) {
	int treeId1, treeId2;
	scanf("%d %d", &treeId1, &treeId2);
	
	Tnode *p = head; // copy tree

	while (p->tree->treeId != treeId1) {
		p = p->next;
	}

	tree *t = malloc (sizeof(tree));
	t->treeId = treeId2;
	t->root = copy_tree(p->tree->root);
	
	printf("copied\n");
	return t;
}

GLnode *copy_tree (GLnode *p) {
	if (p != NULL) {
		GLnode *root = malloc (sizeof(GLnode));
		root->data = p->data;
		root->next = copy_tree(p->next);
		root->down = copy_tree(p->down);
		return root;
	}
	return NULL;
}

bool isequal(Tnode *head) {
	int treeId1, treeId2;
	scanf("%d %d", &treeId1, &treeId2);
	
	Tnode *p = head; // first tree
	Tnode *q = head; // second tree

	while (p->tree->treeId != treeId1) {
		p = p->next;
	}
	while (q->tree->treeId != treeId2) {
		q = q->next;
	}

	return isequal_tree(p->tree->root, q->tree->root);
}

bool isequal_tree(GLnode *p, GLnode *q) {
	if (p != NULL && q != NULL) {
		if (p->data == q->data) {
			if (isequal_tree(p->down, q->down)) {
				if (isequal_tree(p->next, q->next)) {
					return true;
				}
			}
		}
	}
	else if (p == NULL && q == NULL) {
		return true;
	}
	return false;	
}

Tnode *clear (Tnode *head) {
	int treeId;
	scanf("%d", &treeId);
	
	Tnode *prev = NULL, *now;
	now = head;
	while (now->tree->treeId != treeId) {
		prev = now;
		now = now->next;
	}

	if (prev != NULL) {
		prev->next = now->next;
	}
	else {
		head = now->next;
	}
	free(now);
	return head;
}

char *write (Tnode *head) {
	int treeId;
	char *str = malloc (sizeof(char) * MAXSTRINGLENGTH);
	scanf("%d", &treeId);
	
	Tnode *p = head;
	while (p->tree->treeId != treeId) {
		p = p->next;
	}
	
	str[0] = '(';
	write_node(p->tree->root->down, str);
	i = 1;

	return str;
}

void write_node (GLnode *p, char *str) {
	str[i++] = p->data;
	if (p->down != NULL) {
		str[i++] = '(';
		write_node(p->down, str);
	}
	if (p->next != NULL) {
		str[i++] = ',';
		write_node(p->next, str);
	}
	else {
		str[i++] = ')';
	}
}

