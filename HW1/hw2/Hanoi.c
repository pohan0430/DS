#include <stdio.h>

void Hanoi (int n, char a, char b, char c) {
	if (n >= 1) {
		Hanoi(n-1, a, c, b);
		printf("%d %c %c\n", n, a, c);
		Hanoi(n-1, b, a, c);
	}
}

int main () {
	int n;
	scanf("%d", &n);

	Hanoi(n, '1', '2', '3');

	return 0;
}
