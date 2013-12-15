#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {	
	char* cp1 = malloc(16);
	
	int i = 0;
	for (i = 0; i < 0; ++i) {
		cp1[i] = 'q';
	}

	free(cp1-8);

	return 0;
}