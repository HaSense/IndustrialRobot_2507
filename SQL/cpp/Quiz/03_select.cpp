#include <sqlite3.h>
#include <iostream>

int main()
{
    sqlite3 *db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* query1 = "SELECT id, name, age, email FROM student ORDER BY id";
    int rc = sqlite3_open("student.db", &db);

    // 에러처리
    if (rc != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return 1;
    }

    std::cout << "연결 성공\n";

    sqlite3_prepare_v2(db,
                       query1,
                       -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const auto *name = sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);
        bool emailNull = sqlite3_column_type(stmt, 3) == SQLITE_NULL;

        std::cout << id << ", " << name << ", " << age << ", "
                  << (emailNull ? "(없음)" : reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)))
                  << '\n';
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}