/*
    Ubuntu 24.04

    컴파일
    gcc insert_person.c -o insert_person -lsqlite3

    실행
    ./insert_person
*/

#include <stdio.h>
#include <sqlite3.h>

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

    // 데이터 삽입
    const char *sql =
        "INSERT INTO person VALUES"
        "(1,'Hong Gil Dong','010-1234-5678'),"
        "(2,'Lee Sun Shin','010-2345-6789'),"
        "(3,'Kim Yu Shin','010-3456-7890'),"
        "(4,'Park Ji Sung','010-4567-8901'),"
        "(5,'Choi Min Soo','010-5678-9012');";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("데이터 삽입 실패 : %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        printf("데이터 삽입 완료\n");
    }

    // 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}
