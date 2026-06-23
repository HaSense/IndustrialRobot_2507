/*
   Ubuntu 24.04 기준
   설치 
   sudo apt update
   sudo apt install sqlite3 libsqlite3-dev
   
   컴파일 방법
   gcc product_sqlite.c -o product_sqlite -lsqlite3
   실행
   ./product_sqlite
  */
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

typedef struct
{
    int id;
    char name[50];
    int quantity;
    int price;
} Product;

void executeSQL(sqlite3 *db, const char *sql)
{
    char *errMsg = NULL;

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("SQL 오류: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void createTable(sqlite3 *db)
{
    const char *sql =
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "price INTEGER NOT NULL"
        ");";

    executeSQL(db, sql);
}

void insertInitialData(sqlite3 *db)
{
    const char *sql =
        "INSERT OR IGNORE INTO products VALUES "
        "(1001, '타이어', 20, 120000),"
        "(1002, '와이퍼', 35, 15000),"
        "(1003, '엔진오일', 50, 35000),"
        "(1004, '배터리', 12, 90000),"
        "(1005, '브레이크패드', 25, 60000),"
        "(1006, '에어컨필터', 40, 18000),"
        "(1007, '전조등', 18, 45000);";

    executeSQL(db, sql);
}

void printMenu()
{
    printf("\n==== 자동차 부품 재고 관리 프로그램(SQLite) ====\n");
    printf("1. 전체 재고 조회\n");
    printf("2. 부품 검색\n");
    printf("3. 입고 처리\n");
    printf("4. 출고 처리\n");
    printf("5. 신규 부품 등록\n");
    printf("6. 부품 삭제\n");
    printf("0. 종료\n");
    printf("메뉴 선택: ");
}

void printProduct(Product p)
{
    printf("%d\t%-15s\t%d개\t%d원\n",
           p.id, p.name, p.quantity, p.price);
}

void printAll(sqlite3 *db)
{
    const char *sql =
        "SELECT id, name, quantity, price "
        "FROM products ORDER BY id;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    printf("\nID\t부품명\t\t수량\t가격\n");
    printf("----------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Product p;

        p.id = sqlite3_column_int(stmt, 0);
        strcpy(p.name, (const char *)sqlite3_column_text(stmt, 1));
        p.quantity = sqlite3_column_int(stmt, 2);
        p.price = sqlite3_column_int(stmt, 3);

        printProduct(p);
    }

    sqlite3_finalize(stmt);
}

void searchProduct(sqlite3 *db)
{
    int id;

    printf("검색할 부품 ID 입력: ");
    scanf("%d", &id);

    const char *sql =
        "SELECT id, name, quantity, price "
        "FROM products WHERE id = ?;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Product p;

        p.id = sqlite3_column_int(stmt, 0);
        strcpy(p.name, (const char *)sqlite3_column_text(stmt, 1));
        p.quantity = sqlite3_column_int(stmt, 2);
        p.price = sqlite3_column_int(stmt, 3);

        printf("\nID\t부품명\t\t수량\t가격\n");
        printf("----------------------------------------\n");
        printProduct(p);
    }
    else
    {
        printf("해당 부품을 찾을 수 없습니다.\n");
    }

    sqlite3_finalize(stmt);
}

void stockIn(sqlite3 *db)
{
    int id, amount;

    printf("입고할 부품 ID 입력: ");
    scanf("%d", &id);

    printf("입고 수량 입력: ");
    scanf("%d", &amount);

    if (amount <= 0)
    {
        printf("입고 수량은 1개 이상이어야 합니다.\n");
        return;
    }

    const char *sql =
        "UPDATE products "
        "SET quantity = quantity + ? "
        "WHERE id = ?;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) == 0)
        printf("해당 부품을 찾을 수 없습니다.\n");
    else
        printf("입고 처리가 완료되었습니다.\n");
}

void stockOut(sqlite3 *db)
{
    int id, amount;

    printf("출고할 부품 ID 입력: ");
    scanf("%d", &id);

    printf("출고 수량 입력: ");
    scanf("%d", &amount);

    if (amount <= 0)
    {
        printf("출고 수량은 1개 이상이어야 합니다.\n");
        return;
    }

    const char *sql =
        "UPDATE products "
        "SET quantity = quantity - ? "
        "WHERE id = ? AND quantity >= ?;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, amount);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) == 0)
        printf("해당 부품이 없거나 재고가 부족합니다.\n");
    else
        printf("출고 처리가 완료되었습니다.\n");
}

void addProduct(sqlite3 *db)
{
    Product p;

    printf("신규 부품 ID 입력: ");
    scanf("%d", &p.id);

    printf("부품명 입력: ");
    scanf("%s", p.name);

    printf("재고 수량 입력: ");
    scanf("%d", &p.quantity);

    printf("가격 입력: ");
    scanf("%d", &p.price);

    const char *sql =
        "INSERT INTO products(id, name, quantity, price) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, p.id);
    sqlite3_bind_text(stmt, 2, p.name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, p.quantity);
    sqlite3_bind_int(stmt, 4, p.price);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        printf("신규 부품이 등록되었습니다.\n");
    else
        printf("등록 실패: 이미 존재하는 ID일 수 있습니다.\n");

    sqlite3_finalize(stmt);
}

void deleteProduct(sqlite3 *db)
{
    int id;

    printf("삭제할 부품 ID 입력: ");
    scanf("%d", &id);

    const char *sql =
        "DELETE FROM products WHERE id = ?;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) == 0)
        printf("해당 부품을 찾을 수 없습니다.\n");
    else
        printf("부품이 삭제되었습니다.\n");
}

int main()
{
    sqlite3 *db;
    int choice;

    if (sqlite3_open("products.db", &db) != SQLITE_OK)
    {
        printf("DB 연결 실패\n");
        return 1;
    }

    createTable(db);
    insertInitialData(db);

    while (1)
    {
        printMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printAll(db);
            break;

        case 2:
            searchProduct(db);
            break;

        case 3:
            stockIn(db);
            break;

        case 4:
            stockOut(db);
            break;

        case 5:
            addProduct(db);
            break;

        case 6:
            deleteProduct(db);
            break;

        case 0:
            printf("프로그램을 종료합니다.\n");
            sqlite3_close(db);
            return 0;

        default:
            printf("잘못된 메뉴입니다.\n");
            break;
        }
    }

    sqlite3_close(db);

    return 0;
}
