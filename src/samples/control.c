#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 256
#define REALLOC_SIZE 200

int main(int argc, char *argv[]) {
	char *string1 = malloc(MAX_SIZE * sizeof(char));
	char *string2 = calloc(256, sizeof(char));
	char *string3;
	int i; 
	// Assign some values
	for(i = 0; i < MAX_SIZE; i++) {
		string1[i] = 'q';
	}
	// Read and writes
	for(i = 0; i < MAX_SIZE; i ++) {
		string2[i] = string1[i];
	}
	// Use realloc;
	string3 = realloc(string2, REALLOC_SIZE);
	for(i = 0; i < REALLOC_SIZE; i++) {
		string3[i] = 'b';
	}
	// Free memory
	free(string1);
	free(string3);
	return EXIT_SUCCESS;
}