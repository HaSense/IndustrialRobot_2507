/*
    Ubuntu 24.04

    컴파일
    gcc select_person_callback.c -o select_person_callback -lsqlite3

    실행
    ./select_person_callback
*/

#include <stdio.h>
#include <sqlite3.h>

// 조회 결과를 출력하는 콜백 함수
int callback(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s : %s\n",
               azColName[i],
               argv[i] ? argv[i] : "NULL");
    }

    printf("------------------------\n");

    return 0;
}

int main()
{
    sqlite3 *db;
    char *errMsg = NULL;

    // 데이터베이스 열기
    if (sqlite3_open("person.db", &db) != SQLITE_OK)
    {
        printf("데이터베이스 열기 실패\n");
        return 1;
    }

    const char *sql = "SELECT * FROM person;";

    // SQL 실행
    if (sqlite3_exec(db, sql, callback, NULL, &errMsg) != SQLITE_OK)
    {
        printf("조회 실패 : %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    // 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}
