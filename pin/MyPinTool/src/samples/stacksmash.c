#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//gcc -fno-stack-protector mybadprgm.c 

int checkPasswd(char *passwd) {
	int isRight = 0;
	char passBuff[4];

	strcpy(passBuff, passwd);

	if (strcmp(passBuff, "p") == 0)
		isRight = 1;	

	return isRight;

}

int main(int argc, char *argv[]) {
	
	if(argc < 2) {
		printf("you forgot passwd\n");
		exit(1);
	}

	if(checkPasswd(argv[1]))
		printf("correct\n");
	else
		printf("wrong\n");

	return 0;
}
