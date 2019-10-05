#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int *a = (int*)malloc(sizeof(int));
	printf("%p\n", a);
	free(a);
	printf("%p\n", a);
	return 0;
}
