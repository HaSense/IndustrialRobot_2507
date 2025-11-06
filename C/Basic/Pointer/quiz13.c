#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>
void print_max(int* pa);
int main()
{
	int array[7] = { 4,5,8,1,2,3,7 };

	print_max(array);

	return 0;
}
void print_max(int* pa)
{
	int max = INT_MIN; //변수의 Scope
	for (int i = 0; i < 7; i++) {
		
		if (pa[i] > max)
			max = pa[i];
	}

	printf("가장 큰 값 : %d\n", max);
}
