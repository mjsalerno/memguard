#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
	
	printf("###start###\n");
	char* cp1 = malloc(2);
	malloc(8);
	malloc(8);
	malloc(8);
	malloc(8);
	*cp1 = 'h';
	*(cp1 +1)= 'i';

	printf("###stop###\n");
	free(cp1);

	return 0;
}