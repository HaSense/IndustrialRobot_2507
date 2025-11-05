#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int main()
{
	int N, X;
	scanf("%d %d", &N, &X); //3, 5

	//C에서 배열을 동적할당을 하고 싶다면~!!!
	int* A = (int*)malloc(N * sizeof(int));
	
	if (A == NULL) {	//에러처리
		printf("memory error~!!!");
		return -1;
	}

	for (int i = 0; i < N; i++) {	//1, 2, 3
		scanf("%d", &A[i]);
	}
	for (int i = 0; i < N; i++) {
		printf("%d\t", A[i]);	//1, 2, 3
	}


	return 0;
}
