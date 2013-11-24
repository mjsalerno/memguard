#include <stdio.h>
#include <stdlib.h>

int main() {
	int *p = malloc(sizeof(int) * 2); // just allocate 8 bytes (2 int)
	if(p == NULL) {
		exit(EXIT_FAILURE);
	}
	printf("The address %p was returned\n", p);
	p[1] = 6;
	// printf("%d\n", p[0]);
	if(!p) {
		exit(1);
	}
	// free(p);
	return EXIT_SUCCESS;
}