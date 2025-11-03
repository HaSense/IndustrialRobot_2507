#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	for (int i = 0; i < 15; i++) {
		printf("*");
	}
	printf("\n\n");
	//i, j, k, l, m, n ...
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 5; k++) {
			printf("*");
		}
	}
	printf("\n\n");
	for (int l = 0; l < 3; l++) {
		for (int m = 0; m < 5; m++) {
			printf("*");
		}
		printf("\n");
	}

	return 0;
}
