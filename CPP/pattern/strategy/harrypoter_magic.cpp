#include <iostream>
#include <string>
using namespace std;

// 전략 인터페이스
class MagicStrategy {
public:
    virtual void cast() = 0;   // 순수 가상 함수
    virtual ~MagicStrategy() = default;            // 가상 소멸자
};

// 구체 전략: 화염 마법
class FireMagic : public MagicStrategy {
public:
    void cast() override {
        cout << "해리가 화염 마법을 시전합니다!" << endl;
        
    }
};

// 구체 전략: 얼음 마법
class IceMagic : public MagicStrategy {
public:
    void cast() override {
        cout << "해리가 얼음 마법을 시전합니다!" << endl;
       
    }
};

// 컨텍스트: 해리 포터
class Harry {
private:
    MagicStrategy* currentMagic;   // 현재 사용 중인 마법 전략(소유권 없음)

public:
    Harry() : currentMagic(nullptr) {}

    // 전략 교체
    void setMagic(MagicStrategy* magic) {
        currentMagic = magic;
    }

    // 공격 실행
    void attack() {
        if (currentMagic == nullptr) {
            cout << "현재 선택된 마법이 없습니다. 마법을 먼저 선택하세요." << endl;
            return;
        }    
        currentMagic->cast();
    }
};

int main() {
    Harry harry;
    FireMagic fireMagic;   // 구체 전략 객체(스택에 생성)
    IceMagic  iceMagic;

    while (true) {
        cout << "\n=== 해리 포터 마법 전투 시뮬레이터 ===" << endl;
        cout << "1. 화염 마법 선택" << endl;
        cout << "2. 얼음 마법 선택" << endl;
        cout << "3. 공격 실행" << endl;
        cout << "0. 종료" << endl;
        cout << "메뉴 선택 : ";

        int menu;
        if (!(cin >> menu)) {
            cin.clear();
            cout << "잘못된 입력입니다. 숫자를 입력하세요." << endl;
            continue;
        }

        if (menu == 0) {
            cout << "프로그램을 종료합니다..." << endl;
            break;
        }

        switch (menu) {
        case 1:
            cout << "[시스템] 해리가 화염 마법을 선택합니다." << endl;
            harry.setMagic(&fireMagic);
            break;

        case 2:
            cout << "[시스템] 해리가 얼음 마법을 선택합니다." << endl;
            harry.setMagic(&iceMagic);
            break;

        case 3:
            harry.attack();
            break;

        default:
            cout << "잘못된 메뉴입니다. 다시 입력하세요." << endl;
            break;
        }
    }

    return 0;
}
