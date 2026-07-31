#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

using namespace SQLite;
using namespace std;
int main()
{
    try
    {
        SQLite::Database db("student.db",
                            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        std::cout << "student.db 열기 성공" << std::endl;

        db.exec("PRAGMA foreign_keys = ON");
        db.exec("CREATE TABLE IF NOT EXISTS log("
                "id INTEGER PRIMARY KEY, message TEXT NOT NULL)");
        
        int changed = db.exec("DELETE FROM log WHERE id < 0");
        std::cout << "변경 행: " << changed << '\n';
        std::cout << "SQLite 버전: " << db.execAndGet("SELECT sqlite_version()").getString() << '\n';
    }
    catch (const SQLite::Exception &e)
    {
        std::cerr << "데이터베이스 오류: " << e.what() << '\n';
        return 1;
    }
}