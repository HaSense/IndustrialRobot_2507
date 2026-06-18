#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char line[100];

    int day;
    int qty[7];
    int count = 0;

    fp = fopen("production.csv", "r");

    if(fp == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    // 헤더 읽기
    fgets(line, sizeof(line), fp);

    while(fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%d,%d", &day, &qty[count]);
        count++;
    }

    fclose(fp);

    printf("===== 생산량 목록 =====\n");

    for(int i = 0; i < count; i++)
    {
        printf("%d일차 : %d\n", i + 1, qty[i]);
    }

    return 0;
}
