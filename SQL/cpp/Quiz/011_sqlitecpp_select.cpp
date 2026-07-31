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

    void selectStudent()
    {
        string sql_01 = "SELECT id, name, age, email FROM student ORDER BY id";
        SQLite::Statement query(db, sql_01.c_str());

        cout << "\n===== 학생 목록 =====" << endl;

        while (query.executeStep())
        {
            int id = query.getColumn(0).getInt();
            string name = query.getColumn(1).getString();
            int age = query.getColumn(2).getInt();

            auto emailColumn = query.getColumn(3);

            cout << id << " | "
                 << name << " | "
                 << age << " | ";

            if (emailColumn.isNull())
                cout << "(없음)";
            else
                cout << emailColumn.getString();

            cout << endl;
        }
    }
};

int main()
{
    try
    {
        DBManager manager("student.db");
        manager.selectStudent();
    }
    catch (const SQLite::Exception& e)
    {
        cerr << "데이터베이스 오류 : "
             << e.what() << endl;

        return 1;
    }

    return 0;
}