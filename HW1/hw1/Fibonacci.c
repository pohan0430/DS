#include <stdio.h>
#include <stdlib.h>

int Fib_recursive(int i) {
	if (i == 0 || i == 1) {
		return i;
	}
	return Fib_recursive(i-1) + Fib_recursive(i-2);
}

int Fib_iterative(int i) {
	if (i == 0 || i == 1) {
		return i;
	}
	int a = 1, b = 0, c;
	for (; i >= 2; i--) {
		c = a + b;
		b = a;
		a = c;
	}
	return c;
}

int main () {
	int n;
	scanf("%d", &n);
	char **strings = malloc (sizeof(char *) * n);
	int *a = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		strings[i] = malloc (sizeof(char) * 11);
		scanf("%s %d", *(strings+i), a+i);
	}

	for (int i = 0; i < n; i++) {
		if (strings[i][0] == 'i') {
			printf("%d\n", Fib_iterative(a[i]));
		}
		else {
			printf("%d\n", Fib_recursive(a[i]));
		}
	}
	
	return 0;
}

