/*
    Ubuntu 24.04 기준

    설치
    sudo apt update
    sudo apt install sqlite3 libsqlite3-dev

    컴파일
    gcc content_manager.c -o content_manager -lsqlite3

    실행
    ./content_manager

    설명
    - SQLite를 사용하는 C 콘솔 프로그램
    - 영화, 드라마, 책, 음악 등 감상한 콘텐츠를 기록
    - 구조체 기반으로 입력, DB 처리, 메뉴 처리를 분리
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DB_PATH "/home/smart/work/dbfiles/contents.db"

#define TYPE_SIZE 50
#define TITLE_SIZE 100
#define CREATOR_SIZE 100
#define MEMO_SIZE 500
#define DATE_SIZE 30
#define KEYWORD_SIZE 100

/*
    콘텐츠 한 건을 표현하는 구조체
*/
typedef struct
{
    int id;
    char type[TYPE_SIZE];
    char title[TITLE_SIZE];
    char creator[CREATOR_SIZE];
    int rating;
    char memo[MEMO_SIZE];
    char created_at[DATE_SIZE];
} Content;

/*
    프로그램 전체 상태를 관리하는 구조체
*/
typedef struct
{
    sqlite3 *db;
    const char *db_path;
} App;

/* 함수 선언 */
void inputLine(char *buffer, int size);
int inputInt(void);

int initApp(App *app, const char *db_path);
void closeApp(App *app);
int createTable(App *app);

void printMenu(void);
void runApp(App *app);

void inputContent(Content *content);
void inputUpdateData(int *id, int *rating, char *memo, int memo_size);

int insertContent(App *app, const Content *content);
int updateContentDB(App *app, int id, int rating, const char *memo);
int deleteContentDB(App *app, int id);
int findContentById(App *app, int id, Content *content);

void addContent(App *app);
void listContents(App *app);
void viewContent(App *app);
void updateContent(App *app);
void deleteContent(App *app);
void searchContent(App *app);

void printContentSummary(const Content *content);
void printContentDetail(const Content *content);

/*
    문자열 한 줄 입력 함수
    fgets는 개행 문자까지 입력하므로 마지막 개행을 제거한다.
*/
void inputLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

/*
    정수 입력 함수
    scanf 이후 남아 있는 개행 문자를 getchar로 제거한다.
*/
int inputInt(void)
{
    int value;
    scanf("%d", &value);
    getchar();
    return value;
}

/*
    프로그램 초기화
    DB 파일을 열고 테이블을 생성한다.
*/
int initApp(App *app, const char *db_path)
{
    app->db = NULL;
    app->db_path = db_path;

    if (sqlite3_open(app->db_path, &app->db) != SQLITE_OK)
    {
        printf("DB 열기 실패: %s\n", sqlite3_errmsg(app->db));
        return 0;
    }

    if (!createTable(app))
    {
        printf("테이블 생성 실패\n");
        return 0;
    }

    return 1;
}

/*
    프로그램 종료 처리
*/
void closeApp(App *app)
{
    if (app->db != NULL)
    {
        sqlite3_close(app->db);
        app->db = NULL;
    }
}

/*
    contents 테이블 생성
*/
int createTable(App *app)
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

    if (sqlite3_exec(app->db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("SQL 오류: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }

    return 1;
}

/*
    메뉴 출력
*/
void printMenu(void)
{
    printf("\n========== 콘텐츠 감상 기록 관리 ==========" "\n");
    printf("1. 콘텐츠 등록\n");
    printf("2. 콘텐츠 목록 보기\n");
    printf("3. 콘텐츠 상세 조회\n");
    printf("4. 콘텐츠 수정\n");
    printf("5. 콘텐츠 삭제\n");
    printf("6. 제목 검색\n");
    printf("0. 종료\n");
    printf("선택: ");
}

/*
    메인 실행 루프
    while + switch 구조
*/
void runApp(App *app)
{
    int choice;

    while (1)
    {
        printMenu();
        choice = inputInt();

        switch (choice)
        {
        case 1:
            addContent(app);
            break;
        case 2:
            listContents(app);
            break;
        case 3:
            viewContent(app);
            break;
        case 4:
            updateContent(app);
            break;
        case 5:
            deleteContent(app);
            break;
        case 6:
            searchContent(app);
            break;
        case 0:
            printf("프로그램을 종료합니다.\n");
            return;
        default:
            printf("잘못된 메뉴입니다.\n");
            break;
        }
    }
}

/*
    콘텐츠 등록용 입력
*/
void inputContent(Content *content)
{
    memset(content, 0, sizeof(Content));

    printf("종류 입력(영화/드라마/책/음악): ");
    inputLine(content->type, sizeof(content->type));

    printf("제목 입력: ");
    inputLine(content->title, sizeof(content->title));

    printf("제작자 입력(감독/작가/가수 등): ");
    inputLine(content->creator, sizeof(content->creator));

    printf("평점 입력(1~10): ");
    content->rating = inputInt();

    printf("느낀점 입력: ");
    inputLine(content->memo, sizeof(content->memo));
}

/*
    수정용 입력
*/
void inputUpdateData(int *id, int *rating, char *memo, int memo_size)
{
    printf("수정할 ID 입력: ");
    *id = inputInt();

    printf("새 평점 입력(1~10): ");
    *rating = inputInt();

    printf("새 느낀점 입력: ");
    inputLine(memo, memo_size);
}

/*
    INSERT 처리
*/
int insertContent(App *app, const Content *content)
{
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT INTO contents(type, title, creator, rating, memo) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare 실패: %s\n", sqlite3_errmsg(app->db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, content->type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, content->title, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, content->creator, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, content->rating);
    sqlite3_bind_text(stmt, 5, content->memo, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("등록 실패: %s\n", sqlite3_errmsg(app->db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

/*
    UPDATE 처리
*/
int updateContentDB(App *app, int id, int rating, const char *memo)
{
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "UPDATE contents "
        "SET rating = ?, memo = ? "
        "WHERE id = ?;";

    if (sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare 실패: %s\n", sqlite3_errmsg(app->db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, rating);
    sqlite3_bind_text(stmt, 2, memo, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("수정 실패: %s\n", sqlite3_errmsg(app->db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);

    return sqlite3_changes(app->db) > 0;
}

/*
    DELETE 처리
*/
int deleteContentDB(App *app, int id)
{
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "DELETE FROM contents "
        "WHERE id = ?;";

    if (sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare 실패: %s\n", sqlite3_errmsg(app->db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("삭제 실패: %s\n", sqlite3_errmsg(app->db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);

    return sqlite3_changes(app->db) > 0;
}

/*
    ID로 콘텐츠 한 건 조회
    찾으면 Content 구조체에 값을 채우고 1 반환
    없으면 0 반환
*/
int findContentById(App *app, int id, Content *content)
{
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "SELECT id, type, title, creator, rating, memo, created_at "
        "FROM contents "
        "WHERE id = ?;";

    if (sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare 실패: %s\n", sqlite3_errmsg(app->db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        memset(content, 0, sizeof(Content));

        content->id = sqlite3_column_int(stmt, 0);

        snprintf(content->type, sizeof(content->type), "%s", sqlite3_column_text(stmt, 1));
        snprintf(content->title, sizeof(content->title), "%s", sqlite3_column_text(stmt, 2));
        snprintf(content->creator, sizeof(content->creator), "%s", sqlite3_column_text(stmt, 3));
        content->rating = sqlite3_column_int(stmt, 4);
        snprintf(content->memo, sizeof(content->memo), "%s", sqlite3_column_text(stmt, 5));
        snprintf(content->created_at, sizeof(content->created_at), "%s", sqlite3_column_text(stmt, 6));

        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

/*
    콘텐츠 요약 출력
*/
void printContentSummary(const Content *content)
{
    printf("ID: %d | [%s] %s | %s | 평점: %d | %s\n",
           content->id,
           content->type,
           content->title,
           content->creator,
           content->rating,
           content->created_at);
}

/*
    콘텐츠 상세 출력
*/
void printContentDetail(const Content *content)
{
    printf("\nID: %d\n", content->id);
    printf("종류: %s\n", content->type);
    printf("제목: %s\n", content->title);
    printf("제작자: %s\n", content->creator);
    printf("평점: %d\n", content->rating);
    printf("느낀점: %s\n", content->memo);
    printf("기록일: %s\n", content->created_at);
}

/*
    메뉴 1. 콘텐츠 등록
*/
void addContent(App *app)
{
    Content content;

    printf("\n[콘텐츠 등록]\n");

    inputContent(&content);

    if (insertContent(app, &content))
    {
        printf("등록 완료\n");
    }
    else
    {
        printf("등록 실패\n");
    }
}

/*
    메뉴 2. 콘텐츠 목록 보기
*/
void listContents(App *app)
{
    sqlite3_stmt *stmt = NULL;
    Content content;

    const char *sql =
        "SELECT id, type, title, creator, rating, created_at "
        "FROM contents "
        "ORDER BY id DESC;";

    printf("\n[콘텐츠 목록]\n");
    printf("------------------------------------------------------------\n");

    if (sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare 실패: %s\n", sqlite3_errmsg(app->db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        memset(&content, 0, sizeof(Content));

        content.id = sqlite3_column_int(stmt, 0);
        snprintf(content.type, sizeof(content.type), "%s", sqlite3_column_text(stmt, 1));
        snprintf(content.title, sizeof(content.title), "%s", sqlite3_column_text(stmt, 2));
        snprintf(content.creator, sizeof(content.creator), "%s", sqlite3_column_text(stmt, 3));
        content.rating = sqlite3_column_int(stmt, 4);
        snprintf(content.created_at, sizeof(content.created_at), "%s", sqlite3_column_text(stmt, 5));

        printContentSummary(&content);
    }

    printf("------------------------------------------------------------\n");

    sqlite3_finalize(stmt);
}

/*
    메뉴 3. 콘텐츠 상세 조회
*/
void viewContent(App *app)
{
    int id;
    Content content;

    printf("\n[상세 조회]\n");
    printf("조회할 ID 입력: ");
    id = inputInt();

    if (findContentById(app, id, &content))
    {
        printContentDetail(&content);
    }
    else
    {
        printf("해당 ID의 콘텐츠가 없습니다.\n");
    }
}

/*
    메뉴 4. 콘텐츠 수정
*/
void updateContent(App *app)
{
    int id;
    int rating;
    char memo[MEMO_SIZE];

    printf("\n[콘텐츠 수정]\n");

    inputUpdateData(&id, &rating, memo, sizeof(memo));

    if (updateContentDB(app, id, rating, memo))
    {
        printf("수정 완료\n");
    }
    else
    {
        printf("해당 ID가 없거나 수정되지 않았습니다.\n");
    }
}

/*
    메뉴 5. 콘텐츠 삭제
*/
void deleteContent(App *app)
{
    int id;

    printf("\n[콘텐츠 삭제]\n");
    printf("삭제할 ID 입력: ");
    id = inputInt();

    if (deleteContentDB(app, id))
    {
        printf("삭제 완료\n");
    }
    else
    {
        printf("해당 ID가 없습니다.\n");
    }
}

/*
    메뉴 6. 제목 검색
*/
void searchContent(App *app)
{
    char keyword[KEYWORD_SIZE];
    char pattern[KEYWORD_SIZE + 10];
    sqlite3_stmt *stmt = NULL;
    Content content;

    printf("\n[제목 검색]\n");
    printf("검색어 입력: ");
    inputLine(keyword, sizeof(keyword));

    snprintf(pattern, sizeof(pattern), "%%%s%%", keyword);

    const char *sql =
        "SELECT id, type, title, creator, rating, created_at "
        "FROM contents "
        "WHERE title LIKE ? "
        "ORDER BY id DESC;";

    if (sqlite3_prepare_v2(app->db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare 실패: %s\n", sqlite3_errmsg(app->db));
        return;
    }

    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_TRANSIENT);

    printf("\n[검색 결과]\n");
    printf("------------------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        memset(&content, 0, sizeof(Content));

        content.id = sqlite3_column_int(stmt, 0);
        snprintf(content.type, sizeof(content.type), "%s", sqlite3_column_text(stmt, 1));
        snprintf(content.title, sizeof(content.title), "%s", sqlite3_column_text(stmt, 2));
        snprintf(content.creator, sizeof(content.creator), "%s", sqlite3_column_text(stmt, 3));
        content.rating = sqlite3_column_int(stmt, 4);
        snprintf(content.created_at, sizeof(content.created_at), "%s", sqlite3_column_text(stmt, 5));

        printContentSummary(&content);
    }

    printf("------------------------------------------------------------\n");

    sqlite3_finalize(stmt);
}

int main(void)
{
    App app;

    if (!initApp(&app, DB_PATH))
    {
        closeApp(&app);
        return 1;
    }

    runApp(&app);

    closeApp(&app);

    return 0;
}
