#include<stdio.h>

void showAvg(int* arr)
{
	int total = 0;
	for (int i = 0; i < 3; i++)
		total += arr[i];

	printf("%.1lf\n", total / 3.0);
}
int main()
{
	int score[3];
	for (int i = 0; i < 3; i++)
		scanf("%d", &score[i]);

	showAvg(score);
	return 0;
}
