#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	//3단
	for (int i = 1; i < 10; i++) {
		printf("3 * %d = %d\n", i, 3*i);
	}
	printf("\n");
	//A ~ Z
	for (int i = 'A'; i <= 'Z'; i++) {
		printf("%c ", i);
	}
	printf("\n");
	for (int i = 'a'; i <= 'z'; i++) {
		printf("%c ", i);
	}


	return 0;
}
