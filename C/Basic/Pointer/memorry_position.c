#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int total;

int main()
{
	int* pi;
	double* pd;
	int a, b;

	pi = (int*)malloc(sizeof(int));
	printf("Heap : %p\n", pi);
	printf("Stack : %p\t%p\n\n", &a, &b);
	printf("String : %p\n", "hello");
	printf("Global : %p\n", &total);


	return 0;
}



