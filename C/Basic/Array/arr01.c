#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//배열 개념연습 - 가장 쉬운 코드
int main()
{
	int arr[3];

	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;

	printf("%d %d %d\n\n", arr[0], arr[1], arr[2]);
	//////////////////////////////////////////////////////////////
	for (int i = 0; i < 3; i++) {
		arr[i] = i + 5;
	}
	for (int i = 0; i < 3; i++) {
		printf("%d\n", arr[i]);
	}

	return 0;
}
