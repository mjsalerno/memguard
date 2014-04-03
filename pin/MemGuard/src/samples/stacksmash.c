#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//gcc -fno-stack-protector mybadprgm.c 

int checkPasswd() {
	int isRight = 0;
	char passBuff[4];

	strcpy(passBuff, "overwrite the ret");

	if (strcmp(passBuff, "p") == 0)
		isRight = 1;	

	return isRight;

}

int main(int argc, char *argv[]) {

	if(checkPasswd())
		printf("correct\n");
	else
		printf("wrong\n");

	return 0;
}
