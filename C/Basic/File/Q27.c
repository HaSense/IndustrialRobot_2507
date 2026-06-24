#include <stdio.h>

#define FILE_NAME "products.txt"

int main()
{
    FILE *fp;

    fp = fopen(FILE_NAME, "r+");

    if (fp == NULL)
    {
        printf("파일 열기 실패\n");
        return 1;
    }

    // 파일 맨 앞으로 이동
    fseek(fp, 0, SEEK_SET);

    // 헤더 덮어쓰기
    fprintf(fp,
            "id,name,qty,price,lot");

    // 파일 끝으로 이동
    fseek(fp, 0, SEEK_END);

    printf("현재 파일 위치 : %ld\n",
           ftell(fp));

    // 이후 데이터 추가 가능
    // fprintf(fp,
    //         "3,PLC,10,250000,LOT003\n");

    fclose(fp);

    return 0;
}
