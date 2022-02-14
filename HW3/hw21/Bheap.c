#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define tableSize 14
#define OpNum 5000
#define TestCases 7
#define true 1
#define false 0

typedef struct bheap {
	int data, degree;
	struct bheap *child;
	struct bheap *sibling;
}bheap;

typedef struct leftistTree {
	struct leftistTree *leftchild;
	struct leftistTree *rightchild;
	int data, shortest;
}leftistTree;

leftistTree *create_leftistTree (int *, int );
leftistTree *minUnion (leftistTree *, leftistTree *);
leftistTree *insert_leftistTree (leftistTree *, int);
leftistTree *delete_leftistTree (leftistTree *);

bheap *min = NULL;

bheap *create (int);
void insert (int);
void meld ();
int deleteMin ();

int main () {
	srand(time(NULL));
	clock_t start, end;
	int elementNum[TestCases] = {100, 500, 1000, 2000, 3000, 4000, 5000};
	double timeUsed_bheap[7] = {0};
	double timeUsed_Leftist[7] = {0};
	int insertcount[7] = {0};
	int randNum;

	for (int i = 0; i < TestCases; i++) {
		int n = elementNum[i];
		int *tree = malloc (sizeof(leftistTree) * elementNum[i]);

		// initialize random list of element
		for (int j = 1; j <= n; j++) {
			randNum = rand();
			tree[j-1] = randNum;
			insert(randNum);
		}
		
		// initialize leftist tree and bheap
		leftistTree *root = create_leftistTree(tree, n);
		meld();

		// initialize random operation and insert element
		int *op = malloc (sizeof(int) * OpNum);
		int *insertElement = malloc (sizeof(int) * (OpNum+2));
		for (int j = 0, k = 0; j < OpNum; j++) {
			op[j] = rand() % 2;
			if(op[j]) {
				insertcount[i] += 1;
				insertElement[k++] = rand();
			}
		}

		//Leftist
		start = clock();
		for (int j = 0, k = 0; j < OpNum; j++) {
			if (op[j]) {
				root = insert_leftistTree(root, insertElement[k++]);
			}
			else {
				if (root != NULL) {
					root = delete_leftistTree(root);
				}
				else {
					op[j--] = 1;
					insertElement[--k] = rand();
				}
			}
		}
		end = clock();
		timeUsed_Leftist[i] = (double) (end-start)/CLOCKS_PER_SEC;

		//bheap
		start = clock();
		for (int j = 0, k = 0; j < OpNum; j++) {
			if (op[j]) {
				insert(insertElement[k++]);
			}
			else {
				deleteMin();
			}
		}
		end = clock();
		timeUsed_bheap[i] = (double) (end-start)/CLOCKS_PER_SEC;
	
	}
	
	for (int i = 0; i < 7; i++) {
		printf("---------------------------\n");
		printf("n = %d:\n", elementNum[i]);
		printf("Number of insertions: %d\n", insertcount[i]);
		printf("bheap: %lf (second)\n", timeUsed_bheap[i]);
		printf("Leftist: %lf (second)\n", timeUsed_Leftist[i]);
	}

	return 0;
}

bheap *create (int data) {
	bheap *root = malloc (sizeof(bheap));
	root->data = data;
	root->child = NULL;
	root->sibling = NULL;
	root->degree = 0;
	return root;
}

void insert (int data) {
	bheap *new = create(data);
	if (min) {
		new->sibling = min->sibling;
		min->sibling = new;
	}
	else {
		new->sibling = new;
		min = new;
	}
}

int deleteMin () {
	bheap *temp = min;
	int data = temp->data;
	if (min->child && min->sibling != min) {
		while (min->sibling != temp) {
			min = min->sibling;
		}
		min->sibling = temp->child;
		min = min->sibling;
		while (min->sibling != temp->child) {
			min = min->sibling;
		}
		min->sibling = temp->sibling;
	}
	else if (!min->child && min->sibling != min) {
		while (min->sibling != temp) {
			min = min->sibling;
		}
		min->sibling = temp->sibling;
	}
	else if (min->child && min->sibling == min) {
		min = min->child;
	}
	else {
		min = NULL;	
	}
	free(temp);
	
	if (min) {
		meld();
	}

	return data;
}

void meld () {
	bheap **table = calloc (tableSize, sizeof(bheap *));
	bheap *temp1 = min, *temp2, *temp3;

	do {
		temp2 = min->sibling;
		
		// table root point to itself
		min->sibling = min;

		while (table[min->degree]) {
			// if table->data < min->data then swap min and table element
			if (table[min->degree]->data < min->data) {
				temp3 = min;
				min = table[min->degree];
				table[min->degree] = temp3;
			}

			if (min->child) {
				table[min->degree]->sibling = min->child->sibling;
				min->child->sibling = table[min->degree];
			}
			else {
				min->child = table[min->degree];
			}
			table[min->degree] = NULL;
			min->degree++;
		}
		table[min->degree] = min;
		min = temp2;
	}while (min != temp1);

	bheap *first = NULL, *temp = NULL;

	for (int i = 0; i < tableSize; i++) {
		if (table[i]) {
			if (first) {
				if (table[i]->data < min->data) {
					min = table[i];
				}
				temp->sibling = table[i];
				temp = table[i];
			}
			else {
				min = table[i];
				first = table[i];
				temp = first;
			}
		}
	}
	temp->sibling = first;
}

leftistTree *create_leftistTree (int *array, int size) {
	leftistTree *list[size];
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

	return list[0];
}

leftistTree *minUnion (leftistTree *first, leftistTree *second) {
	if (first && second) {
		if (first->data > second->data) { 
			// let first be the smaller key
			leftistTree *temp = first;
			first = second;
			second = temp;
		}

		if (first->leftchild) {
			first->rightchild = minUnion(first->rightchild, second);
			if (first->leftchild->shortest < first->rightchild->shortest) {
				leftistTree *temp = first->leftchild;
				first->leftchild = first->rightchild;
				first->rightchild = temp;
			}
			first->shortest = first->rightchild->shortest + 1;
	    }
		else {		
		    first->leftchild = second;
		}

		return first;
	}
	else if (first) {
		return first;
	}
	else {
		return second;
	}
}

leftistTree *insert_leftistTree (leftistTree *root, int data) {
	leftistTree *second = malloc (sizeof(leftistTree));
	second->leftchild = NULL;
	second->rightchild = NULL;
	second->data = data;
	second->shortest = 1;
	return minUnion(root, second);
}

leftistTree *delete_leftistTree (leftistTree *root) {
	leftistTree *first = root->leftchild, *second = root->rightchild;
	free(root);
	return minUnion(first, second);
}
