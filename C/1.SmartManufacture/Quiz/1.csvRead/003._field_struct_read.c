#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int id;
    char name[50];
    int qty;
    int price;
} Product;

int main()
{
    FILE *fp;
    char line[256];

    fp = fopen("products.csv", "r");

    if (fp == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    // 헤더 건너뛰기
    fgets(line, sizeof(line), fp);

    printf("ID\t이름\t수량\t단가\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        Product p;

        char *id;
        char *name;
        char *qty;
        char *price;

        id = strtok(line, ",");
        name = strtok(NULL, ",");
        qty = strtok(NULL, ",");
        price = strtok(NULL, ",");

        p.id = atoi(id);
        strcpy(p.name, name);
        p.qty = atoi(qty);
        p.price = atoi(price);

        printf("%d\t%s\t%d\t%d\n",
               p.id, p.name, p.qty, p.price);
    }

    fclose(fp);

    return 0;
}
