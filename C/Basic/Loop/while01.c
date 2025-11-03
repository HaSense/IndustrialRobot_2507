#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// 1 ~ 100까지 합 --> while문으로 작성
int main()
{
	int i = 0; //초기값
	int sum = 0;

	while (i < 100) {
		//증감
		i++;
		sum = sum + i;
	}
	//합계를 출력
	printf("%d\n", sum);
	return 0;
}
