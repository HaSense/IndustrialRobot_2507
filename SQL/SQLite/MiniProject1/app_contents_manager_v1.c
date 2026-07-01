/*
    Ubuntu 24.04 기준

    설치
    sudo apt update
    sudo apt install sqlite3 libsqlite3-dev

    컴파일
    gcc content_manager.c -o content_manager -lsqlite3

    실행
    ./content_manager
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void inputLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void createTable(sqlite3 *db)
{
    char *errMsg = NULL;

    const char *sql =
        "CREATE TABLE IF NOT EXISTS contents ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type TEXT NOT NULL,"
        "title TEXT NOT NULL,"
        "creator TEXT,"
        "rating INTEGER,"
        "memo TEXT,"
        "created_at TEXT DEFAULT (datetime('now', 'localtime'))"
        ");";

    if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("테이블 생성 실패: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void addContent(sqlite3 *db)
{
    char type[50];
    char title[100];
    char creator[100];
    char memo[500];
    int rating;

    printf("\n[콘텐츠 등록]\n");

    printf("종류 입력(영화/드라마/책/음악): ");
    inputLine(type, sizeof(type));

    printf("제목 입력: ");
    inputLine(title, sizeof(title));

    printf("제작자 입력(감독/작가/가수 등): ");
    inputLine(creator, sizeof(creator));

    printf("평점 입력(1~10): ");
    scanf("%d", &rating);
    getchar();

    printf("느낀점 입력: ");
    inputLine(memo, sizeof(memo));

    const char *sql =
        "INSERT INTO contents(type, title, creator, rating, memo) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, creator, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, rating);
    sqlite3_bind_text(stmt, 5, memo, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        printf("등록 완료\n");
    }
    else
    {
        printf("등록 실패: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void listContents(sqlite3 *db)
{
    const char *sql =
        "SELECT id, type, title, creator, rating, created_at "
        "FROM contents "
        "ORDER BY id DESC;";

    sqlite3_stmt *stmt;

    printf("\n[콘텐츠 목록]\n");
    printf("------------------------------------------------------------\n");

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *type = sqlite3_column_text(stmt, 1);
        const unsigned char *title = sqlite3_column_text(stmt, 2);
        const unsigned char *creator = sqlite3_column_text(stmt, 3);
        int rating = sqlite3_column_int(stmt, 4);
        const unsigned char *created_at = sqlite3_column_text(stmt, 5);

        printf("ID: %d | [%s] %s | %s | 평점: %d | %s\n",
               id,
               type,
               title,
               creator ? creator : (const unsigned char *)"",
               rating,
               created_at);
    }

    printf("------------------------------------------------------------\n");

    sqlite3_finalize(stmt);
}

void viewContent(sqlite3 *db)
{
    int id;

    printf("\n[상세 조회]\n");
    printf("조회할 ID 입력: ");
    scanf("%d", &id);
    getchar();

    const char *sql =
        "SELECT id, type, title, creator, rating, memo, created_at "
        "FROM contents "
        "WHERE id = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("\nID: %d\n", sqlite3_column_int(stmt, 0));
        printf("종류: %s\n", sqlite3_column_text(stmt, 1));
        printf("제목: %s\n", sqlite3_column_text(stmt, 2));
        printf("제작자: %s\n", sqlite3_column_text(stmt, 3));
        printf("평점: %d\n", sqlite3_column_int(stmt, 4));
        printf("느낀점: %s\n", sqlite3_column_text(stmt, 5));
        printf("기록일: %s\n", sqlite3_column_text(stmt, 6));
    }
    else
    {
        printf("해당 ID의 콘텐츠가 없습니다.\n");
    }

    sqlite3_finalize(stmt);
}

void updateContent(sqlite3 *db)
{
    int id;
    char memo[500];
    int rating;

    printf("\n[콘텐츠 수정]\n");
    printf("수정할 ID 입력: ");
    scanf("%d", &id);
    getchar();

    printf("새 평점 입력(1~10): ");
    scanf("%d", &rating);
    getchar();

    printf("새 느낀점 입력: ");
    inputLine(memo, sizeof(memo));

    const char *sql =
        "UPDATE contents "
        "SET rating = ?, memo = ? "
        "WHERE id = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, rating);
    sqlite3_bind_text(stmt, 2, memo, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
            printf("수정 완료\n");
        else
            printf("해당 ID가 없습니다.\n");
    }
    else
    {
        printf("수정 실패: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void deleteContent(sqlite3 *db)
{
    int id;

    printf("\n[콘텐츠 삭제]\n");
    printf("삭제할 ID 입력: ");
    scanf("%d", &id);
    getchar();

    const char *sql =
        "DELETE FROM contents "
        "WHERE id = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
            printf("삭제 완료\n");
        else
            printf("해당 ID가 없습니다.\n");
    }
    else
    {
        printf("삭제 실패: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void searchContent(sqlite3 *db)
{
    char keyword[100];

    printf("\n[제목 검색]\n");
    printf("검색어 입력: ");
    inputLine(keyword, sizeof(keyword));

    char pattern[120];
    snprintf(pattern, sizeof(pattern), "%%%s%%", keyword);

    const char *sql =
        "SELECT id, type, title, creator, rating "
        "FROM contents "
        "WHERE title LIKE ? "
        "ORDER BY id DESC;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_TRANSIENT);

    printf("\n[검색 결과]\n");

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("ID: %d | [%s] %s | %s | 평점: %d\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_int(stmt, 4));
    }

    sqlite3_finalize(stmt);
}

void printMenu()
{
    printf("\n========== 콘텐츠 감상 기록 관리 ==========\n");
    printf("1. 콘텐츠 등록\n");
    printf("2. 콘텐츠 목록 보기\n");
    printf("3. 콘텐츠 상세 조회\n");
    printf("4. 콘텐츠 수정\n");
    printf("5. 콘텐츠 삭제\n");
    printf("6. 제목 검색\n");
    printf("0. 종료\n");
    printf("선택: ");
}

int main()
{
    sqlite3 *db;
    int choice;
    char *path = "/home/smart/work/dbfiles/contents.db";
    if (sqlite3_open(path, &db) != SQLITE_OK)
    {
        printf("DB 열기 실패: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    createTable(db);

    while (1)
    {
        printMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            addContent(db);
            break;
        case 2:
            listContents(db);
            break;
        case 3:
            viewContent(db);
            break;
        case 4:
            updateContent(db);
            break;
        case 5:
            deleteContent(db);
            break;
        case 6:
            searchContent(db);
            break;
        case 0:
            printf("프로그램을 종료합니다.\n");
            sqlite3_close(db);
            return 0;
        default:
            printf("잘못된 메뉴입니다.\n");
        }
    }

    sqlite3_close(db);
    return 0;
}
