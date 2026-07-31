#include <sqlite3.h>
#include <iostream>

int main() 
{
    sqlite3* db = nullptr;
    int rc = sqlite3_open("student.db", &db);
    
    //에러처리
    if (rc != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return 1;
    }

    std::cout << "연결 성공\n";
    sqlite3_close(db);
}