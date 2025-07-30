#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int cmp(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

int main()
{
	srand(time(NULL)); //rand seed값 설정
	int lotto[7] = { 0, };
	int realLotto[6] = { 0, };
	int bonus = 0;
	int N;
	int cnt = 0;

	//7번 반복
	for (int i = 0; i < 7; i++) {
		cnt = 0;
		N = rand() % 45 + 1;
		//중복값 조사를 위해 2번째 for 생성
		for (int j = 0; j <= i; j++) {
			
			if (lotto[j] == N) {
				cnt++;
				break;
			}
		}
		if (cnt == 0) {
			lotto[i] = N;
		}
		else
		{
			i--;
			continue;
		}
	}

	//출력
	
	memcpy(realLotto, lotto, sizeof(realLotto)); //6개만 복사.
	bonus = lotto[6]; //보너스 번호

	//realLotto 정렬하기
	qsort(realLotto, 6, sizeof(int), cmp);

	printf("로또 번호 : ");
	for(int i = 0; i < 6; i++)
		printf("%4d", realLotto[i]);
	printf("\n\n");

	printf("보너스 번호 : %d\n", bonus);


	return 0;
}


/*
로또 번호 :    8  21  28  31  41  44

보너스 번호 : 22

*/


