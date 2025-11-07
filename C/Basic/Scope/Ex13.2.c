#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int a = 10, b = 20;
	printf("%d %d\n", a, b);
	
	{
		int temp;
		//int a=0, b=0; //지역변수 선언

		temp = a;
		a = b;
		b = temp;
	}
	
	printf("%d %d\n", a, b);
	return 0;
}
