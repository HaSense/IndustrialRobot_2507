#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) 
{
    using namespace std;

    if (argc < 3) {
        cerr << "사용법: " << argv[0] << " <원본파일> <대상파일>\n";
        return 1;
    }

    ifstream inFile(argv[1]); // 기본적으로 텍스트 모드
    if (!inFile.is_open()) {
        cerr << "오류: 원본 파일을 열 수 없습니다.\n";
        return 2;
    }

    ofstream outFile(argv[2]); // 덮어쓰기
    if (!outFile.is_open()) {
        cerr << "오류: 대상 파일을 생성/열 수 없습니다.\n";
        return 3;
    }

    string line;
    while (getline(inFile, line)) {
        outFile << line << "\n";
    }

    cout << "복사 완료: " << argv[1] << " -> " << argv[2] << "\n";
    return 0;
}
