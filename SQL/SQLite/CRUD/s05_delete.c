/*
    delete_person.c

    컴파일
    gcc delete_person.c -o delete_person -lsqlite3
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
        "DELETE FROM person "
        "WHERE id=5;";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("삭제 실패 : %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        printf("데이터 삭제 완료\n");
    }

    sqlite3_close(db);

    return 0;
}
