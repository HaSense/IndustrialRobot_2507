#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 100

typedef struct
{
    int id;
    char name[50];
    int quantity;
    int price;
} Product;

void printMenu()
{
    printf("\n==== 자동차 부품 재고 관리 프로그램 ====\n");
    printf("1. 전체 재고 조회\n");
    printf("2. 부품 검색\n");
    printf("3. 입고 처리\n");
    printf("4. 출고 처리\n");
    printf("5. 신규 부품 등록\n");
    printf("0. 종료\n");
    printf("메뉴 선택: ");
}

void printProduct(Product p)
{
    printf("%d\t%-15s\t%d개\t%d원\n",
           p.id, p.name, p.quantity, p.price);
}

void printAll(Product products[], int count)
{
    printf("\nID\t부품명\t\t수량\t가격\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printProduct(products[i]);
    }
}

int findProductIndex(Product products[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (products[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

void searchProduct(Product products[], int count)
{
    int id;
    printf("검색할 부품 ID 입력: ");
    scanf("%d", &id);

    int index = findProductIndex(products, count, id);

    if (index == -1)
    {
        printf("해당 부품을 찾을 수 없습니다.\n");
    }
    else
    {
        printf("\nID\t부품명\t\t수량\t가격\n");
        printf("----------------------------------------\n");
        printProduct(products[index]);
    }
}

void stockIn(Product products[], int count)
{
    int id, amount;

    printf("입고할 부품 ID 입력: ");
    scanf("%d", &id);

    int index = findProductIndex(products, count, id);

    if (index == -1)
    {
        printf("해당 부품을 찾을 수 없습니다.\n");
        return;
    }

    printf("입고 수량 입력: ");
    scanf("%d", &amount);

    if (amount <= 0)
    {
        printf("입고 수량은 1개 이상이어야 합니다.\n");
        return;
    }

    products[index].quantity += amount;

    printf("입고 처리가 완료되었습니다.\n");
    printf("현재 재고: %d개\n", products[index].quantity);
}

void stockOut(Product products[], int count)
{
    int id, amount;

    printf("출고할 부품 ID 입력: ");
    scanf("%d", &id);

    int index = findProductIndex(products, count, id);

    if (index == -1)
    {
        printf("해당 부품을 찾을 수 없습니다.\n");
        return;
    }

    printf("출고 수량 입력: ");
    scanf("%d", &amount);

    if (amount <= 0)
    {
        printf("출고 수량은 1개 이상이어야 합니다.\n");
        return;
    }

    if (products[index].quantity < amount)
    {
        printf("재고가 부족합니다.\n");
        printf("현재 재고: %d개\n", products[index].quantity);
        return;
    }

    products[index].quantity -= amount;

    printf("출고 처리가 완료되었습니다.\n");
    printf("현재 재고: %d개\n", products[index].quantity);
}

void addProduct(Product products[], int *count)
{
    if (*count >= MAX_ITEMS)
    {
        printf("더 이상 부품을 등록할 수 없습니다.\n");
        return;
    }

    Product p;

    printf("신규 부품 ID 입력: ");
    scanf("%d", &p.id);

    if (findProductIndex(products, *count, p.id) != -1)
    {
        printf("이미 존재하는 ID입니다.\n");
        return;
    }

    printf("부품명 입력: ");
    scanf("%s", p.name);

    printf("재고 수량 입력: ");
    scanf("%d", &p.quantity);

    printf("가격 입력: ");
    scanf("%d", &p.price);

    products[*count] = p;
    (*count)++;

    printf("신규 부품이 등록되었습니다.\n");
}

int main()
{
    Product products[MAX_ITEMS] = {
        {1001, "타이어", 20, 120000},
        {1002, "와이퍼", 35, 15000},
        {1003, "엔진오일", 50, 35000},
        {1004, "배터리", 12, 90000},
        {1005, "브레이크패드", 25, 60000},
        {1006, "에어컨필터", 40, 18000},
        {1007, "전조등", 18, 45000}};

    int count = 7;
    int choice;

    while (1)
    {
        printMenu();
        scanf("%d", &choice);

        if (choice == 1)
        {
            printAll(products, count);
        }
        else if (choice == 2)
        {
            searchProduct(products, count);
        }
        else if (choice == 3)
        {
            stockIn(products, count);
        }
        else if (choice == 4)
        {
            stockOut(products, count);
        }
        else if (choice == 5)
        {
            addProduct(products, &count);
        }
        else if (choice == 0)
        {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        else
        {
            printf("잘못된 메뉴입니다.\n");
        }
    }

    return 0;
}
