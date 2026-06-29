/*
    Ubuntu 24.04

    컴파일
    gcc select_person.c -o select_person -lsqlite3

    실행
    ./select_person
*/

#include <stdio.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    // 데이터베이스 열기
    if (sqlite3_open("person.db", &db) != SQLITE_OK)
    {
        printf("데이터베이스 열기 실패\n");
        return 1;
    }

    const char *sql = "SELECT id, name, phone FROM person;";

    // SQL 준비
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("조회 준비 실패\n");
        sqlite3_close(db);
        return 1;
    }

    printf("ID\tNAME\t\tPHONE\n");
    printf("-------------------------------------------\n");

    // 한 행씩 조회
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        const unsigned char *phone = sqlite3_column_text(stmt, 2);

        printf("%d\t%s\t%s\n", id, name, phone);
    }

    // 마무리
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
