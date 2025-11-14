#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //1970.01.01 00시

int main()
{
	//seed값
	srand((unsigned int)time(NULL)); //random의 초기값을 잡는다.

	for (int i = 0; i < 6; i++) {
		int number = rand() % 45 + 1; //1 ~ 45;
		printf("난수 : %d\n", number);
	}

	return 0;
}



