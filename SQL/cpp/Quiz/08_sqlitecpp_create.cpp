#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

void createSchema(SQLite::Database& db) {
            db.exec(R"sql(
                CREATE TABLE IF NOT EXISTS student (
                    id    INTEGER PRIMARY KEY AUTOINCREMENT,
                    name  TEXT NOT NULL,
                    age   INTEGER NOT NULL CHECK (age >= 0),
                    email TEXT UNIQUE
                )
            )sql");
}

int main() {
    try {
        
        SQLite::Database db("student.db",
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        std::cout << "student.db 열기 성공" << std::endl;

    } catch (const SQLite::Exception& e) {
        std::cerr << "데이터베이스 오류: " << e.what() << std::endl;
        return 1;
    }

    
}