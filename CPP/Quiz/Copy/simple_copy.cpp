// simple_copy.cpp
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
    using namespace std;
    namespace fs = std::filesystem;

    if (argc < 3) {
        cerr << "사용법: " << argv[0] << " <원본파일> <대상파일>\n";
        return 1;
    }

    fs::path src = argv[1];
    fs::path dst = argv[2];

    try {
        if (!fs::exists(src) || !fs::is_regular_file(src)) {
            cerr << "오류: 원본 파일이 존재하지 않거나 일반 파일이 아닙니다.\n";
            return 2;
        }
        // 이미 있으면 덮어쓰기
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
        cout << "복사 완료: " << src << " -> " << dst << "\n";
    } catch (const fs::filesystem_error& e) {
        cerr << "파일시스템 오류: " << e.what() << "\n";
        return 3;
    } catch (const std::exception& e) {
        cerr << "예외 발생: " << e.what() << "\n";
        return 4;
    }
    return 0;
}
