//배열을 매개변수(인자값)로 전달하고
//배열을 return 받아서 사용해 보았다!!!

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

char* function1(char a) {

	return &a;
}
int* function2(int* pa) {
	return pa;
}

int main()
{
	char a = 'Y';
	printf("%p\n", function1(a));
	int arr[3] = { 1, 2, 3 };
	int arr2[3] = { 0, };
	int* pb;
	
	pb = function2(arr);

	for (int i = 0; i < 3; i++) {
		printf("%d\n", pb[i]); //arr[i]
	}

	return 0;
}
