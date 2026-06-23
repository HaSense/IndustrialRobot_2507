/*
    Ubuntu 24.04 기준

    설치
    sudo apt update
    sudo apt install sqlite3 libsqlite3-dev

    컴파일
    gcc product_sqlite.c -o product_sqlite -lsqlite3

    실행
    ./product_sqlite
*/

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

/*
    제품(부품) 1개를 표현하는 구조체

    id       : 제품번호
    name     : 제품명
    quantity : 재고수량
    price    : 단가
*/
typedef struct
{
    int id;
    char name[50];
    int quantity;
    int price;
} Product;


/*
    SQL 실행 전용 함수

    CREATE TABLE
    INSERT
    등 결과를 가져오지 않는 SQL 실행에 사용
*/
void executeSQL(sqlite3 *db, const char *sql)
{
    char *errMsg = NULL;

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("SQL 오류: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}


/*
    products 테이블 생성

    이미 존재하면 생성하지 않음
*/
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


/*
    최초 실행 시 기본 데이터 입력

    INSERT OR IGNORE
    → 같은 ID가 있으면 무시
*/
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


/*
    메뉴 출력
*/
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


/*
    제품 1개 출력

    구조체를 받아 화면에 출력
*/
void printProduct(Product p)
{
    printf("%d\t%-15s\t%d개\t%d원\n",
           p.id, p.name, p.quantity, p.price);
}


/*
    전체 제품 조회 (READ)

    SELECT 결과를 한 행씩 읽어서 출력
*/
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
        strcpy(p.name,
               (const char *)sqlite3_column_text(stmt, 1));
        p.quantity = sqlite3_column_int(stmt, 2);
        p.price = sqlite3_column_int(stmt, 3);

        printProduct(p);
    }

    sqlite3_finalize(stmt);
}


/*
    제품 검색 (READ)

    ID를 기준으로 1개 조회
*/
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

    /*
        ? 위치에 값 바인딩
    */
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Product p;

        p.id = sqlite3_column_int(stmt, 0);
        strcpy(p.name,
               (const char *)sqlite3_column_text(stmt, 1));
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


/*
    입고 처리 (UPDATE)

    quantity 증가
*/
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


/*
    출고 처리 (UPDATE)

    재고가 충분한 경우만 차감
*/
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


/*
    신규 제품 등록 (CREATE)
*/
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


/*
    제품 삭제 (DELETE)
*/
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


/*
    프로그램 시작점

    1. DB 연결
    2. 테이블 생성
    3. 초기 데이터 등록
    4. 메뉴 반복
*/
int main()
{
    ...
}
