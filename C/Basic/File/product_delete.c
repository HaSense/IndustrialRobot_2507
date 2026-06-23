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
    int found = 0;

    printf("삭제할 제품번호 입력 : ");
    scanf("%d", &targetId);

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
        // 삭제 대상이면 파일에 다시 쓰지 않음
        if (p.id == targetId)
        {
            found = 1;
            continue;
        }

        // 삭제 대상이 아닌 데이터만 저장
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
        printf("제품 삭제 완료\n");
    else
        printf("해당 제품번호가 없습니다\n");

    return 0;
}
