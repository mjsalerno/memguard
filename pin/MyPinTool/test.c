#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
	int j = 0;
	char *ptr = NULL;        
	char* cp1 = malloc(16);
	// printf("Address returned = %p\n", cp1);	
    // printf("this will be heap\n");
    *(cp1+9) = 'q';
    // printf("that was heap\n");	
    
    // printf("this will be stack\n");
    j++;
    // printf("that was stack\n");

	int i = 0;
	// write beyond malloced space
	// should generate an error
	for (i = 0; i < 18; ++i) {
            cp1[i] = 'q';
	}
	// Use calloc
	char* cptr = calloc(50, sizeof(char));
	for(i = 0; i < 50; i++) {
		cptr[i] = 'z';
	}
	// Use realloc, might return NULL
	char* cptr2 = realloc(cptr, (55 * sizeof(char)));
	for(; i < 55; i++) {
		cptr[i] = 'r';
	}
	//free(cptr); // This might be a bad call to free since calloc and realloc
	free(cptr2);
	free(cp1);
	free(ptr);  // Free NULL
	return 0;
}
