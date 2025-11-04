#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void myPrint();
int get_num();

int main()
{
	int result;
	
	result = get_num();
	printf("반환값 : %d\n", result);
	
	myPrint();
	return 0;
}
int get_num()
{
	int num;
	printf("양수 입력 : ");
	scanf("%d", &num);

	return num;
}
void myPrint()
{
	printf("Hello World~!\n");
}
