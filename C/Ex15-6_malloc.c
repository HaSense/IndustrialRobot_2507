#include <stdio.h>
#include <stdlib.h>

// 이중 포인터를 이용한 2차원 배열 출력
void printArr(int **pa)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d\t", pa[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	int rows = 3;
	int cols = 4;

	// 행 포인터 배열 동적 할당
	int **arr = (int **)malloc(rows * sizeof(int *));
	if (arr == NULL) {
		perror("malloc failed");
		return 1;
	}

	// 각 행의 열을 동적 할당
	for (int i = 0; i < rows; i++) {
		arr[i] = (int *)malloc(cols * sizeof(int));
		if (arr[i] == NULL) {
			perror("malloc failed");
			// 앞서 할당된 메모리 해제 후 종료
			for (int k = 0; k < i; k++) {
				free(arr[k]);
			}
			free(arr);
			return 1;
		}
	}

	// 값 초기화 (예: 1~12)
	int value = 1;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			arr[i][j] = value++;
		}
	}

	// 배열 출력
	printArr(arr);

	// 메모리 해제
	for (int i = 0; i < rows; i++) {
		free(arr[i]);
	}
	free(arr);

	return 0;
}
