#include <sqlite3.h>
#include <iostream>

int main()
{
    sqlite3* db = nullptr;
    char* error = nullptr;

    // 1. 데이터베이스 열기
    int rc = sqlite3_open("student.db", &db);

    if (rc != SQLITE_OK)
    {
        std::cerr << "데이터베이스 연결 실패: "
                  << sqlite3_errmsg(db) << '\n';

        sqlite3_close(db);
        return 1;
    }

    std::cout << "데이터베이스 연결 성공\n";

    // 2. 테이블 생성 SQL
    const char* ddl = R"(
        CREATE TABLE IF NOT EXISTS student (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            age INTEGER NOT NULL CHECK(age >= 0),
            email TEXT UNIQUE
        );
    )";

    // 3. SQL 실행
    rc = sqlite3_exec(db, ddl, nullptr, nullptr, &error);

    if (rc != SQLITE_OK)
    {
        std::cerr << "테이블 생성 실패: ";

        if (error != nullptr)
        {
            std::cerr << error << '\n';
            sqlite3_free(error);
        }

        sqlite3_close(db);
        return 1;
    }

    std::cout << "student 테이블 생성 성공\n";

    // 4. 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}