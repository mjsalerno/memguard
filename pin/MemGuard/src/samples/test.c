#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
	int j = 0;
	char *ptr = NULL;        
	char* cp1 = malloc(16);
	char* misaligned = malloc(64);
	misaligned+= 2;
	*(cp1+9) = 'q';
    j++;
	int i = 0;
	// write beyond malloced space
	// should generate an error
	for (i = 0; i < 18; ++i) {
        cp1[i] = 'q'; // Overflow write
	}

	for (i = 15; i > -1; --i) {
        cp1[i] = 'q'; // underflow write
	}

	// Use calloc
	char* cptr = calloc(50, sizeof(char));
	for(i = 0; i < 50; i++) {
		cptr[i] = 'z';
	}
	// // Underflow write
	cptr[-1] = 'p';
	// Underflow Read
	char p = cptr[-1];
	// overflow Read
	p = cptr[55];
	// Use realloc, might return NULL
	char* cptr2 = realloc(cptr, (55 * sizeof(char)));
	for(; i < 55; i++) {
		cptr2[i] = 'r';
	}
	// Make an error come up for printf
	printf("%c\n", cptr2[55]);
	// Free lots of stuff
	free(misaligned); // Free a misaligned pointer
	free(cptr);
	free(cptr2); // this pointer is the same as cptr 
	free(cp1);
	free(ptr);  // Free NULL
	return 0;
}
