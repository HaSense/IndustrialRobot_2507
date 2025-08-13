#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class TextFileCopier 
{
private:
    string sourcePath;
    string targetPath;

public:
    TextFileCopier(const string& src, const string& tgt)
        : sourcePath(src), targetPath(tgt) {}

    bool copy() {
        ifstream inFile(sourcePath); // 텍스트 모드
        if (!inFile.is_open()) {
            cerr << "오류: 원본 파일을 열 수 없습니다.\n";
            return false;
        }

        ofstream outFile(targetPath); // 덮어쓰기
        if (!outFile.is_open()) {
            cerr << "오류: 대상 파일을 생성/열 수 없습니다.\n";
            return false;
        }

        string line;
        while (getline(inFile, line)) {
            outFile << line << "\n";
        }

        return true;
    }

    void setSource(const string& src) { sourcePath = src; }
    void setTarget(const string& tgt) { targetPath = tgt; }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "사용법: " << argv[0] << " <원본파일> <대상파일>\n";
        return 1;
    }

    TextFileCopier copier(argv[1], argv[2]);

    if (copier.copy()) {
        cout << "복사 완료: " << argv[1] << " -> " << argv[2] << "\n";
    } else {
        cout << "복사 실패.\n";
    }

    return 0;
}
