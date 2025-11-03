#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	//1단계
	/*int N;
	scanf("%d", &N);*/

	for (int i = 0; i <5; i++) {
		for (int j = 0; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
	printf("\n\n");
	//2단계
	for (int i = 0; i < 5; i++) {
		for (int j = 5; j > i; j--) {
			printf("*");
		}
		printf("\n");
	}
	printf("\n\n");

	//3단계
	for (int i = 0; i < 5; i++) {
		for (int j = 4; j > i; j--) {
			printf(" ");
		}
		for (int k = 0; k <= i; k++) {
			printf("*");
		}
		printf("\n");
	}




	return 0;
}
