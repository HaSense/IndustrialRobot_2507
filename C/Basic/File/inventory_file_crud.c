/*
   컴파일 $> gcc inventory_file_crud.c -o inventory
   실행   $> ./inventory

  */

#include <stdio.h>

#define FILE_NAME "products.txt"

// 제품 구조체 정의
typedef struct
{
    int id;
    char name[50];
    int qty;
    int price;
} Product;

// 함수 선언
void createProduct();
void readProducts();
void updateProduct();
void deleteProduct();

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

// Create : 제품 추가
void createProduct()
{
    FILE *fp;
    Product p;

    fp = fopen(FILE_NAME, "a");

    if (fp == NULL)
    {
        printf("파일 열기 실패\n");
        return;
    }

    printf("\n[제품 추가]\n");

    printf("제품 번호 : ");
    scanf("%d", &p.id);

    printf("제품명 : ");
    scanf("%s", p.name);

    printf("수량 : ");
    scanf("%d", &p.qty);

    printf("가격 : ");
    scanf("%d", &p.price);

    // 구조체 데이터를 파일에 저장
    fprintf(fp, "%d,%s,%d,%d\n",
            p.id,
            p.name,
            p.qty,
            p.price);

    fclose(fp);

    printf("제품이 추가되었습니다.\n");
}

// Read : 제품 전체 조회
void readProducts()
{
     FILE *fp;
    Product p;
    char line[256];
    int lineNo = 0;
    int count = 0;

    fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        printf("등록된 제품이 없습니다.\n");
        return;
    }

    printf("\n[제품 목록]\n");
    printf("번호\t제품명\t수량\t가격\n");
    printf("--------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        lineNo++;

        // 줄 끝 개행 문자 제거
        line[strcspn(line, "\n")] = '\0';

        // 빈 줄은 건너뜀
        if (strlen(line) == 0)
        {
            continue;
        }

        int result = sscanf(line, "%d,%49[^,],%d,%d",
                            &p.id,
                            p.name,
                            &p.qty,
                            &p.price);

        if (result == 4)
        {
            printf("%d\t%s\t%d\t%d\n",
                   p.id,
                   p.name,
                   p.qty,
                   p.price);
            count++;
        }
        else
        {
            printf("\n[데이터 형식 오류]\n");
            printf("줄 번호 : %d\n", lineNo);
            printf("원본 내용 : %s\n", line);
            printf("읽은 항목 수 : %d개\n", result);
            printf("필요한 형식 : 제품번호,제품명,수량,가격\n");
            printf("예시 : 1001,Motor,50,12000\n");
        }
    }

    fclose(fp);

    printf("\n총 %d개의 제품을 조회했습니다.\n", count);
}

// Update : 제품 수량과 가격 수정
void updateProduct()
{
    FILE *src;
    FILE *temp;
    Product p;

    int targetId;
    int found = 0;

    src = fopen(FILE_NAME, "r");
    temp = fopen("temp.txt", "w");

    if (src == NULL || temp == NULL)
    {
        printf("파일 열기 실패\n");
        return;
    }

    printf("\n[제품 수정]\n");
    printf("수정할 제품 번호 : ");
    scanf("%d", &targetId);

    while (fscanf(src, "%d,%49[^,],%d,%d",
                  &p.id,
                  p.name,
                  &p.qty,
                  &p.price) == 4)
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
                p.id,
                p.name,
                p.qty,
                p.price);
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

// Delete : 제품 삭제
void deleteProduct()
{
    FILE *src;
    FILE *temp;
    Product p;

    int targetId;
    int found = 0;

    src = fopen(FILE_NAME, "r");
    temp = fopen("temp.txt", "w");

    if (src == NULL || temp == NULL)
    {
        printf("파일 열기 실패\n");
        return;
    }

    printf("\n[제품 삭제]\n");
    printf("삭제할 제품 번호 : ");
    scanf("%d", &targetId);

    while (fscanf(src, "%d,%49[^,],%d,%d",
                  &p.id,
                  p.name,
                  &p.qty,
                  &p.price) == 4)
    {
        // 삭제 대상이면 임시 파일에 저장하지 않음
        if (p.id == targetId)
        {
            found = 1;
            continue;
        }

        fprintf(temp, "%d,%s,%d,%d\n",
                p.id,
                p.name,
                p.qty,
                p.price);
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
