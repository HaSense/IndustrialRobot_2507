#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int score[10];
	for (int i = 0; i < 10; i++) {
		scanf("%d", &score[i]);
	}

	for (int i = 1; i < 10; i+=2) {
		/*if (score[i] % 2 == 0) {
			printf("%d ", score[i]);
		}*/
		printf("%d ", score[i]);
	}

	return 0;
}
