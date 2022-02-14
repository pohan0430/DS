#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef int bool;

typedef struct ThreadBT {
	bool LeftThread;
	struct ThreadBT *leftchild;
	int data;
	struct ThreadBT *rightchild;
	bool RightThread;
}ThreadBT;

ThreadBT *create (int data) {
	ThreadBT *node = malloc (sizeof(ThreadBT));
	node->data = data;
	node->LeftThread = true;
	node->RightThread = true;
	node->leftchild = NULL;
	node->rightchild = NULL;
	return node;
}

ThreadBT *SearchNode (ThreadBT *, int);
ThreadBT *InsertionLeft (ThreadBT *, int, int);
ThreadBT *InsertionRight (ThreadBT *, int, int);
ThreadBT *Insuc(ThreadBT *);
void Inorder (ThreadBT *);

int main () {
	int n, rootId, newId; // n represents the number of threaded binary tree insertion operation
	char *direction = malloc (sizeof(char) * 7);
	scanf("%d %d", &n, &rootId);
	ThreadBT *HeadNode = create(0);
	HeadNode->LeftThread = false;
	HeadNode->RightThread = false;
	HeadNode->rightchild = HeadNode;

	ThreadBT *root = create(rootId);
	HeadNode->leftchild = root;
	root->rightchild = HeadNode;
	root->leftchild = HeadNode;

	for (int i = 0; i < n; i++) {
		scanf("%d %s %d", &rootId, direction, &newId);
		if (!strcmp("left", direction)) {
			HeadNode = InsertionLeft(HeadNode, rootId, newId);
		}
		else {
			HeadNode = InsertionRight(HeadNode, rootId, newId);
		}
	}
	
	Inorder(HeadNode);
	printf("\n");

	return 0;
}

ThreadBT *SearchNode (ThreadBT *root, int parentId) {
	ThreadBT *p = root;
	while (1) {
		p = Insuc(p);
		if (p->data == parentId) {
			break;
		}
	}
	return p;
}

ThreadBT *InsertionLeft (ThreadBT *root, int parentId, int childId) {
	ThreadBT *new = create(childId);  
	ThreadBT *parent = SearchNode(root, parentId);
	new->leftchild = parent->leftchild;
	parent->leftchild = new;
	new->rightchild = parent;
	parent->LeftThread = false;

	return root;
}

ThreadBT *InsertionRight (ThreadBT *root, int parentId, int childId) {
	ThreadBT *new = create(childId);  
	ThreadBT *parent = SearchNode(root, parentId);
	new->rightchild = parent->rightchild;
	parent->rightchild = new;
	new->leftchild = parent;
	parent->RightThread = false;

	return root;
}

ThreadBT *Insuc(ThreadBT *root) {
	ThreadBT *p = root->rightchild;
	if (!root->RightThread) {
		while (!p->LeftThread) {
			p = p->leftchild;
		}
	}
	return p;
}

void Inorder (ThreadBT *root) {
	ThreadBT *p = root;
	while (1) {
		p = Insuc(p);
		if (p == root) {
			break;
		}
		printf("%d ", p->data);
	}
}
