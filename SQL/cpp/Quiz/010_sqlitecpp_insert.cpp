#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>

using namespace std;

class DBManager
{
private:
    SQLite::Database db;

public:
    explicit DBManager(const string& filename)
        : db(filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
    {
    }

    void createSchema()
    {
        db.exec(R"sql(
            CREATE TABLE IF NOT EXISTS student (
                id    INTEGER PRIMARY KEY AUTOINCREMENT,
                name  TEXT NOT NULL,
                age   INTEGER NOT NULL CHECK (age >= 0),
                email TEXT UNIQUE
            );
        )sql");
    }

    void insertStudent(const string& name, int age, const string& email)
    {
        SQLite::Statement insert(
            db,
            R"sql(
                INSERT INTO student(name, age, email)
                VALUES (?, ?, ?);
            )sql"
        );

        insert.bind(1, name);
        insert.bind(2, age);
        insert.bind(3, email);

        int changed = insert.exec();

        cout << changed << "행 등록\n";
        cout << "새 ID: " << db.getLastInsertRowid() << endl;
    }
};

int main()
{
    try
    {
        DBManager manager("student.db");
        manager.createSchema();
        manager.insertStudent("이순신", 45, "lee@example.com");

        cout << "데이터베이스 작업 완료" << endl;
    }
    catch (const SQLite::Exception& e)
    {
        cerr << "데이터베이스 오류: "
             << e.what() << endl;

        return 1;
    }

    return 0;
}