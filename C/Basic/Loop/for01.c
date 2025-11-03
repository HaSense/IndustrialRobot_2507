#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	//1 ~ 100 더하면 누적합 5050 됩니다. 이를 for문
	int sum = 0;
	
	for (int i = 1; i<=100; i++) {
		sum += i;
		printf("sum : %d, i : %d\n", sum, i);
	}
	//printf("sum : %d\n", sum);
	return 0;
}
