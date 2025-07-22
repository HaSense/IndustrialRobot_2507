#include<stdio.h>
#include<limits.h>

int main()
{
	int numbers[9] = { 0, };	//배열을 0으로 초기화
	int cnt=0;
	//int numbers[9] = { 3, 29, 38, 12, 57, 74, 40, 85, 61 };
	int max = INT_MIN;
	for (int i = 0; i < 9; i++) {
		scanf("%d", &numbers[i]);
	}

	for (int j = 0; j < 9; j++) {
		if (numbers[j] > max) {
			max = numbers[j];
			cnt = j;
		}	
	}
	printf("%d\n", max);
	printf("%d\n", cnt + 1);
	return 0;
}
