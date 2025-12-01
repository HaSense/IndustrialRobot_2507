#pragma once 

#include <iostream>
#include <string>
using namespace std;

class Sorcerer {
private:
    string name;        // 이름 (필수)
    string leftWeapon;  // 왼손 무기
    string rightWeapon; // 오른손 무기 (방패)
    string leftRing;    // 왼손 반지
    string rightRing;   // 오른손 반지
    string armor;       // 갑옷
    string amulet;      // 목걸이

    // 외부에서 직접 생성하지 못하도록 private 생성자
    Sorcerer(const string& name,
            const string& leftWeapon,
            const string& rightWeapon,
            const string& leftRing,
            const string& rightRing,
            const string& armor,
            const string& amulet)
        : name(name),
          leftWeapon(leftWeapon),
          rightWeapon(rightWeapon),
          leftRing(leftRing),
          rightRing(rightRing),
          armor(armor),
          amulet(amulet)
    {
    }

public:
    // === Builder 정의 ===
    class Builder {
    private:
        // 필수 값
        string name;

        // 선택 값들에 기본값 설정
        string leftWeapon  = "맨손";
        string rightWeapon = "맨손";
        string leftRing    = "없음";
        string rightRing   = "없음";
        string armor       = "천 로브";
        string amulet      = "없음";

    public:
        // 필수 값은 생성자에서 받도록 강제
        Builder(const string& name)
            : name(name) {}

        // 체이닝 메서드들
        Builder& setLeftWeapon(const string& weapon) {
            leftWeapon = weapon;
            return *this;
        }

        Builder& setRightWeapon(const string& weapon) {
            rightWeapon = weapon;
            return *this;
        }

        Builder& setLeftRing(const string& ring) {
            leftRing = ring;
            return *this;
        }

        Builder& setRightRing(const string& ring) {
            rightRing = ring;
            return *this;
        }

        Builder& setArmor(const string& armorName) {
            armor = armorName;
            return *this;
        }

        Builder& setAmulet(const string& amuletName) {
            amulet = amuletName;
            return *this;
        }

        // 최종 Paladin 객체 생성
        Sorcerer build() const {
            return Sorcerer(
                name,
                leftWeapon,
                rightWeapon,
                leftRing,
                rightRing,
                armor,
                amulet
            );
        }
    };

    void print() const {
        cout << "===== 소서리스 장비 상태 =====\n";
        cout << "이름        : " << name << "\n";
        cout << "왼손 무기   : " << leftWeapon << "\n";
        cout << "오른손 무기 : " << rightWeapon << "\n";
        cout << "왼손 반지   : " << leftRing << "\n";
        cout << "오른손 반지 : " << rightRing << "\n";
        cout << "갑옷        : " << armor << "\n";
        cout << "목걸이      : " << amulet << "\n";
        cout << "===========================\n";
    }
};

