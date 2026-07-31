#include <sqlite3.h>
#include <iostream>

int main()
{
    sqlite3 *db = nullptr;
    int rc = sqlite3_open("student.db", &db);
    // 에러처리
    if (rc != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return 1;
    }
    std::cout << "연결 성공\n";

    // ########  삽입   #########################################
    const char *sql = "DELETE FROM student WHERE id = ?";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << sqlite3_errmsg(db) << '\n';
        return 1;
    }
    int id;
    std::cout << "삭제할 id 값은 : ";
    std::cin >> id;
    sqlite3_bind_int(stmt, 1, id);
    

    //쿼리동작 에러처리
    if (sqlite3_step(stmt) != SQLITE_DONE)
        std::cerr << sqlite3_errmsg(db) << '\n';

    std::cout << "삭제 쿼리 성공!!!" << std::endl;
    //리소스 반환
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
