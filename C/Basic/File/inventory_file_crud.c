/*
   컴파일 $> gcc inventory_file_crud.c -o inventory
   실행   $> ./inventory
*/

#include <stdio.h>
#include <string.h>

#define FILE_NAME "products.txt"

typedef struct
{
    int id;
    char name[50];
    int qty;
    int price;
} Product;

void createProduct();
void readProducts();
void updateProduct();
void deleteProduct();

int parseProduct(char line[], Product *p);

int main()
{
    int menu;

    while (1)
    {
        printf("\n===== 재고 관리 프로그램 =====\n");
        printf("1. 제품 추가\n");
        printf("2. 제품 조회\n");
        printf("3. 제품 수정\n");
        printf("4. 제품 삭제\n");
        printf("0. 종료\n");
        printf("메뉴 선택 : ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            createProduct();
            break;
        case 2:
            readProducts();
            break;
        case 3:
            updateProduct();
            break;
        case 4:
            deleteProduct();
            break;
        case 0:
            printf("프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 메뉴입니다.\n");
            break;
        }
    }

    return 0;
}

int parseProduct(char line[], Product *p)
{
    line[strcspn(line, "\n")] = '\0';

    return sscanf(line, "%d,%49[^,],%d,%d",
                  &p->id,
                  p->name,
                  &p->qty,
                  &p->price);
}

void createProduct()
{
    FILE *fp;
    Product p;

    fp = fopen(FILE_NAME, "a");

    if (fp == NULL)
    {
        perror("파일 열기 실패");
        return;
    }

    printf("\n[제품 추가]\n");

    printf("제품 번호 : ");
    scanf("%d", &p.id);

    printf("제품명 : ");
    scanf("%49s", p.name);

    printf("수량 : ");
    scanf("%d", &p.qty);

    printf("가격 : ");
    scanf("%d", &p.price);

    fprintf(fp, "%d,%s,%d,%d\n",
            p.id, p.name, p.qty, p.price);

    fclose(fp);

    printf("제품이 추가되었습니다.\n");
}

void readProducts()
{
    FILE *fp;
    Product p;
    char line[256];
    int count = 0;
    int lineNo = 0;

    fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        perror("파일 열기 실패");
        return;
    }

    printf("\n[제품 목록]\n");
    printf("번호\t제품명\t수량\t가격\n");
    printf("--------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        lineNo++;

        if (parseProduct(line, &p) == 4)
        {
            printf("%d\t%s\t%d\t%d\n",
                   p.id, p.name, p.qty, p.price);
            count++;
        }
        else
        {
            printf("[형식 오류] %d번째 줄: %s\n", lineNo, line);
        }
    }

    fclose(fp);

    printf("총 %d개 조회\n", count);
}

void updateProduct()
{
    FILE *src;
    FILE *temp;
    Product p;
    char line[256];

    int targetId;
    int found = 0;

    src = fopen(FILE_NAME, "r");
    temp = fopen("temp.txt", "w");

    if (src == NULL || temp == NULL)
    {
        perror("파일 열기 실패");
        return;
    }

    printf("\n[제품 수정]\n");
    printf("수정할 제품 번호 : ");
    scanf("%d", &targetId);

    while (fgets(line, sizeof(line), src) != NULL)
    {
        if (parseProduct(line, &p) == 4)
        {
            if (p.id == targetId)
            {
                printf("새 수량 : ");
                scanf("%d", &p.qty);

                printf("새 가격 : ");
                scanf("%d", &p.price);

                found = 1;
            }

            fprintf(temp, "%d,%s,%d,%d\n",
                    p.id, p.name, p.qty, p.price);
        }
    }

    fclose(src);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("제품 정보가 수정되었습니다.\n");
    else
        printf("해당 제품 번호가 없습니다.\n");
}

void deleteProduct()
{
    FILE *src;
    FILE *temp;
    Product p;
    char line[256];

    int targetId;
    int found = 0;

    src = fopen(FILE_NAME, "r");
    temp = fopen("temp.txt", "w");

    if (src == NULL || temp == NULL)
    {
        perror("파일 열기 실패");
        return;
    }

    printf("\n[제품 삭제]\n");
    printf("삭제할 제품 번호 : ");
    scanf("%d", &targetId);

    while (fgets(line, sizeof(line), src) != NULL)
    {
        if (parseProduct(line, &p) == 4)
        {
            if (p.id == targetId)
            {
                found = 1;
                continue;
            }

            fprintf(temp, "%d,%s,%d,%d\n",
                    p.id, p.name, p.qty, p.price);
        }
    }

    fclose(src);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found)
        printf("제품이 삭제되었습니다.\n");
    else
        printf("해당 제품 번호가 없습니다.\n");
}
