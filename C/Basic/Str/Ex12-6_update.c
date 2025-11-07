#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	char strAge[10];
	char name[20];

	printf("나이 입력 : ");
	fgets(strAge, 10, stdin); // 17  -- > "17"
	int age = atoi(strAge);

	printf("이름 입력 : ");
	fgets(name, 20, stdin);
	printf("이름 : %s\n", name);

	return 0;
}





