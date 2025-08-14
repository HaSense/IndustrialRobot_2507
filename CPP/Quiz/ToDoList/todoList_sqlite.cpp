#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

// 할 일(Todo) 항목을 표현하는 구조체
struct TodoItem {
    int id;
    string title;
    bool completed;
};

// 데이터베이스 관리를 위한 클래스
class DatabaseManager {
public:
    DatabaseManager(const string& dbPath) : db(nullptr), dbPath(dbPath) {}
    ~DatabaseManager() { close(); }

    bool open() {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc != SQLITE_OK) {
            cerr << "데이터베이스를 열 수 없습니다: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        return true;
    }

    void close() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    void createTable() {
        const char* sql = "CREATE TABLE IF NOT EXISTS todos("
                          "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "TITLE TEXT NOT NULL,"
                          "COMPLETED INTEGER NOT NULL DEFAULT 0);";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL 오류: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    void insertTodo(const string& title) {
        string sql = "INSERT INTO todos (TITLE) VALUES ('" + title + "');";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL 오류: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    vector<TodoItem> getTodos() {
        vector<TodoItem> todos;
        const char* sql = "SELECT ID, TITLE, COMPLETED FROM todos;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                TodoItem item;
                item.id = sqlite3_column_int(stmt, 0);
                item.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                item.completed = (sqlite3_column_int(stmt, 2) != 0);
                todos.push_back(item);
            }
        }
        sqlite3_finalize(stmt);
        return todos;
    }

    void updateTodoStatus(int id, bool completed) {
        string sql = "UPDATE todos SET COMPLETED = " + to_string(completed) + " WHERE ID = " + to_string(id) + ";";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL 오류: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    void deleteTodo(int id) {
        string sql = "DELETE FROM todos WHERE ID = " + to_string(id) + ";";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL 오류: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

private:
    sqlite3* db;
    string dbPath;
};

// 애플리케이션의 전체적인 로직을 관리하는 클래스
class TodoApp {
public:
    TodoApp(const string& dbPath) : dbManager(dbPath) {}

    void run() {
        if (!dbManager.open()) {
            return;
        }

        dbManager.createTable();
        int choice;

        while (true) {
            displayMenu();
            cout << "> ";
            cin >> choice;

            if (cin.fail()) {
                cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
                cin.clear();
                clearInputBuffer();
                continue;
            }

            switch (choice) {
                case 1:
                    addTodo();
                    break;
                case 2:
                    listTodos();
                    break;
                case 3:
                    completeTodo();
                    break;
                case 4:
                    deleteTodo();
                    break;
                case 5:
                    cout << "프로그램을 종료합니다." << endl;
                    return;
                default:
                    cout << "알 수 없는 명령어입니다. 1~5 사이의 숫자를 입력해주세요." << endl;
            }
        }
    }

private:
    DatabaseManager dbManager;

    void displayMenu() {
        cout << "\n--- 할 일 관리 프로그램 ---" << endl;
        cout << "1. 할 일 추가" << endl;
        cout << "2. 할 일 목록 보기" << endl;
        cout << "3. 할 일 완료하기" << endl;
        cout << "4. 할 일 삭제하기" << endl;
        cout << "5. 종료" << endl;
        cout << "--------------------------" << endl;
    }

    void clearInputBuffer() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void addTodo() {
        cout << "새로운 할 일을 입력하세요: ";
        string title;
        clearInputBuffer();
        getline(cin, title);
        dbManager.insertTodo(title);
        cout << "할 일이 성공적으로 추가되었습니다." << endl;
    }

    void listTodos() {
        vector<TodoItem> todos = dbManager.getTodos();
        cout << "--- 할 일 목록 ---" << endl;
        if (todos.empty()) {
            cout << "목록에 할 일이 없습니다." << endl;
        } else {
            for (const auto& item : todos) {
                cout << item.id << ". [" << (item.completed ? "완료" : "진행 중") << "] " << item.title << endl;
            }
        }
        cout << "-----------------" << endl;
    }

    void completeTodo() {
        cout << "완료할 할 일의 ID를 입력하세요: ";
        int id;
        cin >> id;
        if (cin.fail()) {
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            cin.clear();
            clearInputBuffer();
            return;
        }
        dbManager.updateTodoStatus(id, true);
        cout << "할 일이 완료 상태로 변경되었습니다." << endl;
    }

    void deleteTodo() {
        cout << "삭제할 할 일의 ID를 입력하세요: ";
        int id;
        cin >> id;
        if (cin.fail()) {
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            cin.clear();
            clearInputBuffer();
            return;
        }
        dbManager.deleteTodo(id);
        cout << "할 일이 성공적으로 삭제되었습니다." << endl;
    }
};

int main() {
    TodoApp app("todos.db");
    app.run();
    return 0;
}
