#include <iostream>
#include <string>
using namespace std;

// 디아블로 성기사(잘못된 빌더 패턴 예제용)
class BadPaladin {
public:
    string name;        // 이름
    string leftWeapon;  // 왼손 무기
    string rightWeapon; // 오른손 무기 (방패 포함)
    string leftRing;    // 왼손 반지
    string rightRing;   // 오른손 반지
    string armor;       // 갑옷
    string amulet;      // 목걸이

    // 기본 생성자: 아무 보호도 없음
    BadPaladin() = default;

    void print() const {
        cout << "[BadPaladin 상태]\n";
        cout << "이름        : " << name << "\n";
        cout << "왼손 무기   : " << leftWeapon << "\n";
        cout << "오른손 무기 : " << rightWeapon << "\n";
        cout << "왼손 반지   : " << leftRing << "\n";
        cout << "오른손 반지 : " << rightRing << "\n";
        cout << "갑옷        : " << armor << "\n";
        cout << "목걸이      : " << amulet << "\n";
    }
};

// 안 좋은 빌더 예제
class BadPaladinBuilder {
private:
    BadPaladin& paladin; // 이미 만들어진 객체를 직접 참조

public:
    // 외부에서 만든 객체를 빌더가 들고만 있음
    BadPaladinBuilder(BadPaladin& p) : paladin(p) {}

    BadPaladinBuilder& setName(const string& name) {
        paladin.name = name;
        return *this;
    }
    BadPaladinBuilder& setLeftWeapon(const string& weapon) {
        paladin.leftWeapon = weapon;
        return *this;
    }
    BadPaladinBuilder& setRightWeapon(const string& weapon) {
        paladin.rightWeapon = weapon;
        return *this;
    }
    BadPaladinBuilder& setLeftRing(const string& ring) {
        paladin.leftRing = ring;
        return *this;
    }
    BadPaladinBuilder& setRightRing(const string& ring) {
        paladin.rightRing = ring;
        return *this;
    }
    BadPaladinBuilder& setArmor(const string& armor) {
        paladin.armor = armor;
        return *this;
    }
    BadPaladinBuilder& setAmulet(const string& amulet) {
        paladin.amulet = amulet;
        return *this;
    }

    // build()가 별 의미가 없음. 이미 paladin을 계속 바꾸고 있음
    BadPaladin& build() {
        return paladin; // 그냥 참조 반환
    }
};

int main_bad() {
    BadPaladin paladin; // 기본 생성 (불완전한 상태)

    // 빌더를 쓴다고 했지만, 사실상 setter 체이닝과 다를 바 없음
    BadPaladinBuilder builder(paladin);
    builder.setName("햄딘")
           .setLeftWeapon("홀(스피릿)")
           .setRightWeapon("방패(헤럴드)")
           .setLeftRing("레번")
           .setRightRing("패캐 링")
           .setArmor("수수께끼")
           .setAmulet("마라의 만화경")
           .build(); // 거의 의미 없음

    paladin.print();

    // 빌더를 넘겨받지 않고도 누구나 paladin을 막 바꿀 수 있음
    paladin.leftWeapon = "맨손";
    paladin.print();

    return 0;
}
