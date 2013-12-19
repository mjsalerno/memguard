#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
	int j = 0;
	char *ptr = NULL;        
	char* cp1 = malloc(16);
	*(cp1+9) = 'q';
    j++;
	int i = 0;
	// write beyond malloced space
	// should generate an error
	for (i = 0; i < 18; ++i) {
        cp1[i] = 'q'; // Overflow write
	}
	// Use calloc
	char* cptr = calloc(50, sizeof(char));
	for(i = 0; i < 50; i++) {
		cptr[i] = 'z';
	}
	// // Underflow write
	cptr[-1] = 'p';
	// // Underflow Read
	char p = cptr[-1];
	// Use realloc, might return NULL
	char* cptr2 = realloc(cptr, (55 * sizeof(char)));
	for(; i < 55; i++) {
		cptr2[i] = 'r';
	}
	// free(cptr);
	free(cptr2);
	free(cp1);
	free(ptr);  // Free NULL
	return 0;
}
