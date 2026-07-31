#include <sqlite3.h>
#include <iostream>

int main()
{
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_open("student.db", &db) != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << '\n';
        return 1;
    }

    const char* sql =
        "UPDATE student "
        "SET name = ? "
        "WHERE id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, "홍길동", -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, 1);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << sqlite3_errmsg(db) << '\n';
    }
    else
    {
        std::cout << "수정된 행 수: "
                  << sqlite3_changes(db) << '\n';
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
