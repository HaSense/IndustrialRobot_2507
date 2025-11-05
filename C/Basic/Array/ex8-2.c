#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int score[5];
	int total = 0;

	for (int i = 0; i < 5; i++) {
		scanf("%d", &score[i]);
		total += score[i];
		printf("%5d", score[i]);
	}

	double avg = (double)total / 5.0;

	printf("\n");
	printf("평균 : %.1lf\n", avg);

	return 0;
}
