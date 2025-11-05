#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>

int main()
{
	int arr[6] = { 3, 7, 2, 9, 5, 1 };
	int max = INT_MIN;
	int index = 0;

	for (int i = 0; i < 6; i++) {
		if (arr[i] > max) {
			max = arr[i];
			index = i;
		}
	}
	printf("max : %d\n", max);
	printf("index : %d\n", index);

	return 0;
}
