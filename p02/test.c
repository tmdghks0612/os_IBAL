#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cc(char **p, int n);

int main() {
	char arr[100] = "123 456 789";
	char *p[100];

	p[0] = strtok(arr, " ");

	cc(p, 2);

	for (int i = 0 ; i < 3; ++i)
		printf("%s\n", p[i]);

	return 0;
}

void cc(char **p, int n) {
	for (int i = 1; i <= n; ++i)
		p[i] = strtok(NULL, " ");
}
