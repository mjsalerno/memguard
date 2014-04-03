#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {
	int *p = malloc(4); // just allocate 4 bytes (1 int)
	if(!p) {
		exit(1);
	}
	printf("Before Free\n");
	free(p);
	return EXIT_SUCCESS;
}
