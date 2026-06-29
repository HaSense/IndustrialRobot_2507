/*
    Ubuntu 24.04

    컴파일
    gcc create_person.c -o create_person -lsqlite3

    실행
    ./create_person
*/

#include <stdio.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db;
    char *errMsg = NULL;

    // person.db 열기 (없으면 자동 생성)
    if (sqlite3_open("person.db", &db) != SQLITE_OK)
    {
        printf("데이터베이스 열기 실패\n");
        return 1;
    }

    // person 테이블 생성
    const char *sql =
        "CREATE TABLE IF NOT EXISTS person ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "phone TEXT"
        ");";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("테이블 생성 실패 : %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        printf("person 테이블 생성 완료\n");
    }

    // 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}
