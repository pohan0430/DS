#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define FunctionLength 8
#define ExpressionNameLength 51

typedef struct node {
	int coef;
	int expon;
	struct node *next;
}node;

typedef struct Expression {
	char *ExpressionName;
	node *expression;
}Expression;

Expression *create (char *, node *);
Expression *pread ();
void pwrite(char *, Expression **);
Expression *padd (Expression **);
Expression *psub (Expression **);
Expression *pmult (Expression **);
void eval(Expression **);	
void perase(Expression **);

int n; // number of commands
int expressNum = 0; // number of expressions

int main () {
	char *functions = malloc(sizeof(char) * FunctionLength);
	char *ExpressionName = malloc(sizeof(char) * ExpressionNameLength);
	char c;

	scanf("%d%c", &n, &c);

	Expression **ExpressionList = malloc (sizeof(Expression*) * n);
	
	for (int i = 0; i < n; i++) {
		scanf("%s", functions);
		if (!strcmp("pread", functions)) {
			Expression *e1 = pread();
			ExpressionList[expressNum] = e1;
			expressNum++;
			printf("ok\n");
		}
		else if (!strcmp("pwrite", functions)) {
			scanf("%s", ExpressionName);
			pwrite(ExpressionName, ExpressionList);
		}
		else if (!strcmp("padd", functions)) {
			Expression *e1 = padd(ExpressionList);
			ExpressionList[expressNum] = e1;
			expressNum++;
			printf("added\n");
		}
		else if (!strcmp("psub", functions)) {
			Expression *e1 = psub(ExpressionList);
			ExpressionList[expressNum] = e1;
			expressNum++;
			printf("subtracted\n");
		}
		else if (!strcmp("pmult", functions)) {
			Expression *e1 = pmult(ExpressionList);
			ExpressionList[expressNum] = e1;
			expressNum++;
			printf("multiplied\n");
		}
		else if (!strcmp("eval", functions)) {
			eval(ExpressionList);	
		}
		else {
			perase(ExpressionList);
		}
		if (i < n-1) {
			scanf("%c", &c);
		}
	}	

	return 0;
}

Expression *create (char *name, node *ex) {
	Expression *e = malloc (sizeof(Expression));
	e->ExpressionName = malloc (sizeof(char) * ExpressionNameLength);
	e->ExpressionName = name;
	e->expression = ex;
	return e;
}

Expression *pread () {
	int terms, coef, expon;
	char *ExpressionName1 = malloc(sizeof(char) * ExpressionNameLength);
	node *head = malloc(sizeof(node));
	head->coef = 0;
	head->expon = 0;
	head->next = NULL;
	node *last = head;

	scanf("%s %d", ExpressionName1, &terms);
	for (int i = 0; i < terms; i++) {
		scanf("%d %d", &coef, &expon);
		node *new = malloc (sizeof(node));
		new->coef = coef;
		new->expon = expon;
		last->next = new;
		last = new;
	}
	last->next = head;

	Expression *e = create(ExpressionName1, head);
	return e;
}

void pwrite(char *ExpressionName, Expression **ExpressionList) {
	int i = 0;
	
	while(strcmp(ExpressionName, ExpressionList[i]->ExpressionName)) {
		i++;
	}
	
	for (node *p = ExpressionList[i]->expression->next; p != ExpressionList[i]->expression; p = p->next) {
		if (p->next == ExpressionList[i]->expression) {
			printf("%dx^%d\n", p->coef, p->expon);
		}
		else {
			printf("%dx^%d+", p->coef, p->expon);
		}
	}
}

Expression *padd (Expression **ExpressionList) {
	char *ExpressionName1 = malloc(sizeof(char) * ExpressionNameLength);
	char *ExpressionName2 = malloc(sizeof(char) * ExpressionNameLength);
	char *ResultName = malloc(sizeof(char) * ExpressionNameLength);
	int count = 0; // count expression match
	Expression *ex1, *ex2, *result;

	scanf("%s %s %s", ResultName, ExpressionName1, ExpressionName2);

	for (int i = 0; i < expressNum; i++) {
		if (!strcmp(ExpressionName1, ExpressionList[i]->ExpressionName)) {
			ex1 = ExpressionList[i];
			count++;
		}
		else if (!strcmp(ExpressionName2, ExpressionList[i]->ExpressionName)) {
			ex2 = ExpressionList[i];
			count++;
		}
		if (count == 2) {
			break;
		}
	}

	node *first = ex1->expression->next;
	node *second = ex2->expression->next;
	node *head = malloc (sizeof(node));
	head->coef = 0;
	head->expon = 0;
	head->next = NULL;
	node *last = head;
	
	while (first != ex1->expression && second != ex2->expression) {
		node *new = malloc (sizeof(node));
		if (first->expon > second->expon) {
		       new->expon = first->expon;
		       new->coef = first->coef;
		       first = first->next;
		}
		else if (first->expon < second->expon) {
		       new->expon = second->expon;
		       new->coef = second->coef;
		       second = second->next;
		}
		else {
			new->expon = first->expon;
			new->coef = first->coef + second->coef;
			first = first->next;
			second = second->next;
		}
		last->next = new;
	        last = new;
	}
	
	while (first != ex1->expression) {
		node *new = malloc (sizeof(node));
        	new->expon = first->expon;
		new->coef = first->coef;
        	first = first->next;
		last->next = new;
		last = new;
	}
	while (second != ex2->expression) {
		node *new = malloc (sizeof(node));
        	new->expon = second->expon;
		new->coef = second->coef;
        	second = second->next;
		last->next = new;
		last = new;
	}
	last->next = head;

	result = create(ResultName, head);
	return result;
}

Expression *psub (Expression **ExpressionList) {
	char *ExpressionName1 = malloc(sizeof(char) * ExpressionNameLength);
	char *ExpressionName2 = malloc(sizeof(char) * ExpressionNameLength);
	char *ResultName = malloc(sizeof(char) * ExpressionNameLength);
	int count = 0; // count expression match
	Expression *ex1, *ex2, *result;

	scanf("%s %s %s", ResultName, ExpressionName1, ExpressionName2);

	for (int i = 0; i < expressNum; i++) {
		if (!strcmp(ExpressionName1, ExpressionList[i]->ExpressionName)) {
			ex1 = ExpressionList[i];
			count++;
		}
		else if (!strcmp(ExpressionName2, ExpressionList[i]->ExpressionName)) {
			ex2 = ExpressionList[i];
			count++;
		}
		if (count == 2) {
			break;
		}
	}

	node *first = ex1->expression->next;
	node *second = ex2->expression->next;
	node *head = malloc (sizeof(node));
	head->coef = 0;
	head->expon = 0;
	head->next = NULL;
	node *last = head;
	
	while (first != ex1->expression && second != ex2->expression) {
		node *new = malloc (sizeof(node));
		if (first->expon > second->expon) {
		       new->expon = first->expon;
		       new->coef = first->coef;
		       first = first->next;
		}
		else if (first->expon < second->expon) {
		       new->expon = second->expon;
		       new->coef = -second->coef;
		       second = second->next;
		}
		else {
			new->expon = first->expon;
			new->coef = first->coef - second->coef;
			first = first->next;
			second = second->next;
		}
		last->next = new;
	        last = new;
	}
	
	while (first != ex1->expression) {
		node *new = malloc (sizeof(node));
        	new->expon = first->expon;
		new->coef = first->coef;
        	first = first->next;
		last->next = new;
		last = new;
	}
	while (second != ex2->expression) {
		node *new = malloc (sizeof(node));
        	new->expon = second->expon;
		new->coef = -second->coef;
        	second = second->next;
		last->next = new;
		last = new;
	}
	last->next = head;

	result = create(ResultName, head);
	return result;
}

Expression *pmult (Expression **ExpressionList) {
	char *ExpressionName1 = malloc(sizeof(char) * ExpressionNameLength);
	char *ExpressionName2 = malloc(sizeof(char) * ExpressionNameLength);
	char *ResultName = malloc(sizeof(char) * ExpressionNameLength);
	int count = 0; // count expression match
	Expression *ex1, *ex2, *result;

	scanf("%s %s %s", ResultName, ExpressionName1, ExpressionName2);

	for (int i = 0; i < expressNum; i++) {
		if (!strcmp(ExpressionName1, ExpressionList[i]->ExpressionName)) {
			ex1 = ExpressionList[i];
			count++;
		}
		else if (!strcmp(ExpressionName2, ExpressionList[i]->ExpressionName)) {
			ex2 = ExpressionList[i];
			count++;
		}
		if (count == 2) {
			break;
		}
	}

	node *head = malloc (sizeof(node));
	head->coef = 0;
	head->expon = 0;
	head->next = head;
	
	int flag = 0; // true represent make new node
	for (node *p = ex1->expression->next; p != ex1->expression; p = p->next) {
		node *last = head;
		for (node *q = ex2->expression->next; q != ex2->expression; q = q->next) {
			do {
				if ((p->expon + q->expon) != last->expon && (p->expon + q->expon) > last->next->expon) {
					node *new = malloc (sizeof(node));
					new->coef = p->coef * q->coef;
					new->expon = p->expon + q->expon;
					new->next = last->next;
					last->next = new;
					last = new;
					flag = 1;
					break;

				}
				last = last->next;
			}while ((p->expon + q->expon) != last->expon);
			if ((p->expon + q->expon) == last->expon && !flag) {
				last->coef += p->coef * q->coef;
			}
			flag = 0;
		}
	}
		        	
	result = create(ResultName, head);
	return result;
}

void eval (Expression **ExpressionList) {
	char *ExpressionName = malloc (sizeof(char) * ExpressionNameLength);
	double x;
	scanf("%s %lf", ExpressionName, &x);
	Expression *ex1;
	double sum = 0;

	for (int i = 0; i < expressNum; i++) {
		if (!strcmp(ExpressionName, ExpressionList[i]->ExpressionName)) {
			ex1 = ExpressionList[i];
		}
	}

	for (node *p = ex1->expression->next; p != ex1->expression; p = p->next) {
		double total = 1;
		for (int i = 0; i < p->expon; i++) {
			total *= x;
		}
		total *= p->coef;
		sum += total;
	}

	printf("%.2lf\n", sum);
}	

void perase(Expression **ExpressionList) {
	char *ExpressionName = malloc (sizeof(char) * ExpressionNameLength);
	Expression *ex1;
	scanf("%s", ExpressionName);

	for (int i = 0; i < expressNum; i++) {
		if (!strcmp(ExpressionName, ExpressionList[i]->ExpressionName)) {
			ex1 = ExpressionList[i];
		}
	}
	
	strcpy(ex1->ExpressionName, "NULLLLLL");

	node *p = ex1->expression->next;
	ex1->expression = NULL;

	printf("erased\n");
}


