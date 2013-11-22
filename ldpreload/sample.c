#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {
	printf("Calling from main...\n");
	int *p = malloc(12); // just allocate 10 bytes
	if(!p) {
		printf("Got allocation error...\n");
		exit(1);
	}
	printf("returning to main..\n");
	free(p);
	printf("freeing memory..\n");
	return EXIT_SUCCESS;
}