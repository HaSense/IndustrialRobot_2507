#include <stdio.h>

int main()
{
    FILE *fp;

    fp = fopen("a.txt", "r");
    if(fp == NULL){
        printf("에러 발생!!!");
        return 1;
    }
    printf("파일이 정상적으로 열렸습니다.\n");
    //읽기
    int ch;
    while(1){
        ch = fgetc(fp);
        if(ch == EOF)
            break;
        putchar(ch);
    }
    printf("\n");

    fclose(fp);

    return 0;
}
