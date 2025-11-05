#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int N = 5;
	//scanf("%d", &N); //5

	int dp[10]; //값 저장 배열, 변수에 값을 저장!

	dp[0] = 1; //0번지 초기화
	for (int i = 1; i < 10; i++) {
		//점화식
		dp[i] = i * dp[i - 1]; //f(N) = N * f(N-1)
	}
	printf("%d! = %d\n", N, dp[N]);

	return 0;
}
