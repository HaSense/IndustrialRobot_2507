/*
    update_person.c

    컴파일
    gcc update_person.c -o update_person -lsqlite3
*/

#include <stdio.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db;
    char *errMsg = NULL;

    if (sqlite3_open("person.db", &db) != SQLITE_OK)
    {
        printf("데이터베이스 열기 실패\n");
        return 1;
    }

    const char *sql =
        "UPDATE person "
        "SET name='Kim Min Soo', "
        "phone='010-9999-9999' "
        "WHERE id=3;";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("수정 실패 : %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        printf("데이터 수정 완료\n");
    }

    sqlite3_close(db);

    return 0;
}
