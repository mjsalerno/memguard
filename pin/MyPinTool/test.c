#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {	
	char* cp1 = malloc(16);
	
	int i = 0;
	for (i = 0; i < 16; ++i) {
		cp1[i] = 'q';
	}


	printf("%s\n", cp1);

	free(cp1);

	return 0;
}