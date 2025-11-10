#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int arr[3][3] = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d\t", arr[i][j]);
		}
		printf("\n");
	}

	int arr2[7][7] = { 0, };
	int cnt = 1;
	printf("\n\n");
	for (int m = 0; m < 7; m++) {
		for (int n = 0; n < 7; n++) {
			arr2[m][n] = cnt++;
			printf("%d\t", arr2[m][n]);
		}
		printf("\n");
	}

	return 0;
}


