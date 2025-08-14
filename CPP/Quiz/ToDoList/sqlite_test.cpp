#pragma comment(lib,"sqlite3.lib")
#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
using namespace std;
int main()
{
	string path="C:\\Users\\shha\\source\\repos\\SqliteTest01\\SqliteTest01\\test.db3";
	//string path = "test.db3";
	sqlite3* db;
	int retv;

	retv = sqlite3_open(path.c_str(), &db);
	if (retv) {
		cout << "DB Open 성공!!";
		sqlite3_close(db);
	}
	else {
		cout << "DB Open 실패";
	}

	return 0;
}

// sqlite3.h 헤더에 추가
// 프로젝트 폴더에 sqlite3.dll, sqlite3.lib 프로젝트 폴더로 복사
