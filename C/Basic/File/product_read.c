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

    fp = fopen("products.txt", "r");

    if (fp == NULL)
    {
        printf("파일 열기 실패\n");
        return 1;
    }

    printf("번호\t제품명\t수량\n");
    printf("--------------------------\n");

    // 파일에서 구조체 단위로 읽기
    while (fscanf(fp, "%d,%49[^,],%d",
                  &p.id,
                  p.name,
                  &p.qty) == 3)
    {
        printf("%d\t%s\t%d\n",
               p.id,
               p.name,
               p.qty);
    }

    fclose(fp);

    return 0;
}
