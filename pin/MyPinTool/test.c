#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
	
        int j = 0;        
	char* cp1 = malloc(16);
        
        printf("this will be heap\n");
        *(cp1+9) = 'q';
        printf("that was heap\n");	
        
        printf("this will be stack\n");
        j++;
        printf("that was stack\n");

	int i = 0;
	for (i = 0; i < 0; ++i) {
		cp1[i] = 'q';
	}

	free(cp1-8);
	return 0;
}