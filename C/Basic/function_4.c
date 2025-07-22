#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//plus, minus, multiple, divide (double return)
int plus(int x, int y);
int minus(int x, int y);
int multiple(int x, int y);
double divide(int x, int y);

int main()
{
	int a = 5, b = 7;
	printf("plus : %d\n", plus(a, b));
	printf("minus : %d\n", minus(a, b));
	printf("multiple : %d\n", multiple(a, b));
	printf("divide : %.2llf\n", divide(a, b));

	return 0;
}
int plus(int x, int y)
{
	return x + y;
}
int minus(int x, int y)
{
	return x - y;
}
int multiple(int x, int y)
{
	return x * y;
}
double divide(int x, int y)
{
	return (double)x / y;
}
