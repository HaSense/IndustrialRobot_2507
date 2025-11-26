#include <iostream>
using namespace std;


// ====================================
// 1. 얕은 복사
// ====================================
class TwinShallow {
private:
    // 모든 객체가 공유하는 정적 변수
    static int d1;
    static int d2;

public:
    TwinShallow(int a, int b) {
        d1 = a;
        d2 = b;
    }

    // 복사 생성자 - 아무 일을 하지 않아도 어차피 static 데이터 공유
    TwinShallow(const TwinShallow& other) {}

    void set(int a, int b) {
        d1 = a;
        d2 = b;
    }

    void print(const char* name) const {
        cout << name << " : (" << d1 << ", " << d2 << ")  [공유 static]\n";
    }
};
// static 멤버 초기화
int TwinShallow::d1 = 0;
int TwinShallow::d2 = 0;


// ====================================
// 2. 깊은 복사 (객체마다 독립된 값)
// ====================================
class TwinDeep {
private:
    int d1, d2;

public:
    TwinDeep(int a, int b) : d1(a), d2(b) {}

    // 깊은 복사 생성자: 값 그대로 복사
    TwinDeep(const TwinDeep& other) {
        d1 = other.d1;
        d2 = other.d2;
    }

    TwinDeep* clone() const {
        return new TwinDeep(*this);
    }

    void set(int a, int b) {
        d1 = a;
        d2 = b;
    }

    void print(const char* name) const {
        cout << name << " : (" << d1 << ", " << d2 << ")\n";
    }
};


// ====================================
// main
// ====================================
int main() {
    cout << "=== 얕은 복사: TwinShallow (static 공유) ===\n";

    TwinShallow s1(10, 20);
    TwinShallow s2 = s1;   // 얕은 복사: 자동 공유

    s1.print("s1 초기");
    s2.print("s2 초기");

    cout << "\n[s2.set(100, 200) 호출]\n";
    s2.set(100, 200);

    s1.print("s1 변경 후");
    s2.print("s2 변경 후");


    cout << "\n=== 깊은 복사: TwinDeep (독립) ===\n";

    TwinDeep* d1 = new TwinDeep(10, 20);
    TwinDeep* d2 = d1->clone();

    d1->print("d1 초기");
    d2->print("d2 초기");

    cout << "\n[d2.set(100, 200) 호출]\n";
    d2->set(100, 200);

    d1->print("d1 변경 후");
    d2->print("d2 변경 후");

    delete d2;

    return 0;
}
