#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// 5! = 5 * 4 * 3 * 2 * 1 -- 120
//      5 * f(4)
//      5 * 4 * f(3) ..
//               3 * f(2)
//                     2  * 1
int factorial(int N)
{
	if (N > 0)
		return N * factorial(N - 1);
	else
		return 1;
}

int main()
{
	int result = factorial(5);
	printf("result = %d\n", result);

	return 0;
}
