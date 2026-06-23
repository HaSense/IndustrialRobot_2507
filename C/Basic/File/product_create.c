#include <stdio.h>

// 제품 정보를 저장할 구조체
typedef struct
{
    int id;
    char name[50];
    int qty;
} Product;

int main()
{
    FILE *fp;
    Product p;

    // 사용자 입력
    printf("제품번호 입력 : ");
    scanf("%d", &p.id);

    printf("제품명 입력 : ");
    scanf("%s", p.name);

    printf("수량 입력 : ");
    scanf("%d", &p.qty);

    // 파일을 추가 모드로 열기
    fp = fopen("products.txt", "a");

    if (fp == NULL)
    {
        printf("파일 열기 실패\n");
        return 1;
    }

    // 구조체 데이터를 파일에 저장
    fprintf(fp, "%d,%s,%d\n", p.id, p.name, p.qty);

    fclose(fp);

    printf("제품 추가 완료\n");

    return 0;
}
