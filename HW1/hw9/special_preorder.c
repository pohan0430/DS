#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define MAXLENGTH 15

typedef int bool;

typedef struct {
	int *s;
	int size;
	int top;
}Stack;

Stack create_stack (int n) {
	Stack s1;
	s1.size = n;
	s1.top = -1;
	s1.s = malloc (sizeof(int) * n);
	return s1;
}

bool isFull (Stack s1) {
	return s1.top == s1.size-1;
}

bool isEmpty (Stack s1) {
	return s1.top == -1;
}

bool push(Stack *s1, int item) {
	if (isFull(*s1)) {
		return false;
	}
	else {
		s1->s[++s1->top] = item;
		return true;
	}
}

int pop (Stack *s1) {
	if (isEmpty(*s1)) {
		return false;
	}
	else {
		return s1->s[s1->top--];
	}
}

void nonrecursivepreorder (int *);
void nonrecursivepostorder (int *);

int n; // how many numbers

int main () {
	scanf("%d", &n);
	int *spList = malloc (sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		scanf("%d", spList+i);
	}
	
	nonrecursivepreorder(spList);
	printf("\n");
	nonrecursivepostorder(spList);
	printf("\n");

	return 0;
}

void nonrecursivepreorder (int *spList) {
	for (int i = 0; i < n; i++) {
		if (spList[i] != -1) {
			printf("%d ", spList[i]);
		}
	}
}

void nonrecursivepostorder (int *spList) {
	Stack s1 = create_stack(n);
	for (int i = 0; i < n; i++) {
		push(&s1, spList[i]);
		while (s1.s[s1.top] == -1 && s1.s[s1.top-1] == -1) {
			pop(&s1);
			pop(&s1);
			printf("%d ", pop(&s1));
			push(&s1, -1);
			if (s1.top == 0 && s1.s[s1.top] == -1) {
				break;
			}
		}
	}	
}






















