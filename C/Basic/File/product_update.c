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
    FILE *src;
    FILE *temp;

    Product p;

    int targetId;
    int newQty;
    int found = 0;

    printf("수정할 제품번호 입력 : ");
    scanf("%d", &targetId);

    printf("새 수량 입력 : ");
    scanf("%d", &newQty);

    src = fopen("products.txt", "r");
    temp = fopen("temp.txt", "w");

    if (src == NULL || temp == NULL)
    {
        printf("파일 열기 실패\n");
        return 1;
    }

    // 원본 파일을 한 줄씩 읽음
    while (fscanf(src, "%d,%49[^,],%d",
                  &p.id,
                  p.name,
                  &p.qty) == 3)
    {
        // 수정 대상 제품이면 수량 변경
        if (p.id == targetId)
        {
            p.qty = newQty;
            found = 1;
        }

        // 변경된 구조체 데이터를 임시 파일에 저장
        fprintf(temp, "%d,%s,%d\n",
                p.id,
                p.name,
                p.qty);
    }

    fclose(src);
    fclose(temp);

    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found)
        printf("제품 수정 완료\n");
    else
        printf("해당 제품번호가 없습니다\n");

    return 0;
}
