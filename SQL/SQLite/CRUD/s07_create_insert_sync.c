/*
    Ubuntu 24.04

    컴파일
    gcc person_create_insert.c -o person_create_insert -lsqlite3

    실행
    ./person_create_insert
*/

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db;
    char *errMsg = NULL;

    if (sqlite3_open("/home/smart/work/dbfiles/person.db", &db) != SQLITE_OK)
    {
        printf("데이터베이스 열기 실패 : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // DB가 잠겨 있으면 최대 5초까지 기다림 !!!!
    sqlite3_busy_timeout(db, 5000);

    // 동시 읽기/쓰기 성능 개선용
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    const char *create_sql =
        "CREATE TABLE IF NOT EXISTS person2 ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "phone TEXT"
        ");";

    if (sqlite3_exec(db, create_sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("테이블 생성 실패 : %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    const char *insert_sql =
        "BEGIN IMMEDIATE;"
        "INSERT INTO person2(name, phone) VALUES('Hong Gil Dong', '010-1111-2222');"
        "COMMIT;";

    if (sqlite3_exec(db, insert_sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("데이터 삽입 실패 : %s\n", errMsg);

        sqlite3_free(errMsg);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        sqlite3_close(db);

        return 1;
    }

    printf("테이블 생성 및 데이터 삽입 성공\n");

    sqlite3_close(db);

    return 0;
}
