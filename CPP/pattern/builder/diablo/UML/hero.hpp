#pragma once

#include <iostream>
#include <string>
#include <memory>

using namespace std;

// -----------------------------
// Product (추상 기반 클래스): Hero
// -----------------------------
class Hero {
protected:
    string name;        // 영웅 이름
    string className;   // 직업 명 (Paladin, Sorceress, Barbarian 등)

    // 장비 슬롯들
    string leftHand;
    string rightHand;
    string leftRing;
    string rightRing;
    string armor;
    string amulet;

public:
    virtual ~Hero() = default;

    // 공통 setter들 (Builder에서만 주로 사용)
    void setName(const string& n)         { name = n; }
    void setClassName(const string& c)    { className = c; }
    void setLeftHand(const string& v)     { leftHand = v; }
    void setRightHand(const string& v)    { rightHand = v; }
    void setLeftRing(const string& v)     { leftRing = v; }
    void setRightRing(const string& v)    { rightRing = v; }
    void setArmor(const string& v)        { armor = v; }
    void setAmulet(const string& v)       { amulet = v; }

    virtual void print() const {
        cout << "===== 영웅 장비 상태 =====\n";
        cout << "직업        : " << className << "\n";
        cout << "이름        : " << name << "\n";
        cout << "왼손        : " << leftHand << "\n";
        cout << "오른손      : " << rightHand << "\n";
        cout << "왼손 반지   : " << leftRing << "\n";
        cout << "오른손 반지 : " << rightRing << "\n";
        cout << "갑옷        : " << armor << "\n";
        cout << "목걸이      : " << amulet << "\n";
        cout << "===========================\n";
    }
};

// -----------------------------
// 구체 Product들: Paladin, Sorceress, Barbarian
// -----------------------------
class Paladin : public Hero {
public:
    Paladin() {
        setClassName("Paladin");
    }
};

class Sorceress : public Hero {
public:
    Sorceress() {
        setClassName("Sorceress");
    }
};

class Barbarian : public Hero {
public:
    Barbarian() {
        setClassName("Barbarian");
    }
};

// -----------------------------
// Builder (추상 인터페이스 역할): HeroBuilder
// -----------------------------
class HeroBuilder {
public:
    virtual ~HeroBuilder() = default;

    virtual void reset() = 0;

    virtual void setName(const string& name) = 0;
    virtual void equipLeftHand(const string& item) = 0;
    virtual void equipRightHand(const string& item) = 0;
    virtual void equipLeftRing(const string& item) = 0;
    virtual void equipRightRing(const string& item) = 0;
    virtual void equipArmor(const string& item) = 0;
    virtual void equipAmulet(const string& item) = 0;

    // 최종 Hero 반환
    virtual unique_ptr<Hero> getResult() = 0;
};

// -----------------------------
// ConcreteBuilder: PaladinBuilder
// -----------------------------
class PaladinBuilder : public HeroBuilder {
private:
    unique_ptr<Paladin> hero;

public:
    PaladinBuilder() {
        reset();
    }

    void reset() override {
        hero = make_unique<Paladin>();
        // 팔라딘 기본 장비
        hero->setName("이름없는 팔라딘");
        hero->setLeftHand("맨손");
        hero->setRightHand("방패 없음");
        hero->setLeftRing("없음");
        hero->setRightRing("없음");
        hero->setArmor("천 갑옷");
        hero->setAmulet("없음");
    }

    void setName(const string& name) override {
        hero->setName(name);
    }

    void equipLeftHand(const string& item) override {
        hero->setLeftHand(item);
    }

    void equipRightHand(const string& item) override {
        hero->setRightHand(item);
    }

    void equipLeftRing(const string& item) override {
        hero->setLeftRing(item);
    }

    void equipRightRing(const string& item) override {
        hero->setRightRing(item);
    }

    void equipArmor(const string& item) override {
        hero->setArmor(item);
    }

    void equipAmulet(const string& item) override {
        hero->setAmulet(item);
    }

    unique_ptr<Hero> getResult() override {
        return std::move(hero);
    }
};

// -----------------------------
// ConcreteBuilder: SorceressBuilder
// -----------------------------
class SorceressBuilder : public HeroBuilder {
private:
    unique_ptr<Sorceress> hero;

public:
    SorceressBuilder() {
        reset();
    }

    void reset() override {
        hero = make_unique<Sorceress>();
        hero->setName("이름없는 소서리스");
        hero->setLeftHand("맨손");
        hero->setRightHand("보조 없음");
        hero->setLeftRing("없음");
        hero->setRightRing("없음");
        hero->setArmor("천 로브");
        hero->setAmulet("없음");
    }

    void setName(const string& name) override {
        hero->setName(name);
    }

    void equipLeftHand(const string& item) override {
        hero->setLeftHand(item);
    }

    void equipRightHand(const string& item) override {
        hero->setRightHand(item);
    }

    void equipLeftRing(const string& item) override {
        hero->setLeftRing(item);
    }

    void equipRightRing(const string& item) override {
        hero->setRightRing(item);
    }

    void equipArmor(const string& item) override {
        hero->setArmor(item);
    }

    void equipAmulet(const string& item) override {
        hero->setAmulet(item);
    }

    unique_ptr<Hero> getResult() override {
        return std::move(hero);
    }
};

// -----------------------------
// ConcreteBuilder: BarbarianBuilder
// -----------------------------
class BarbarianBuilder : public HeroBuilder {
private:
    unique_ptr<Barbarian> hero;

public:
    BarbarianBuilder() {
        reset();
    }

    void reset() override {
        hero = make_unique<Barbarian>();
        hero->setName("이름없는 바바리안");
        hero->setLeftHand("맨손");
        hero->setRightHand("맨손");
        hero->setLeftRing("없음");
        hero->setRightRing("없음");
        hero->setArmor("가죽 갑옷");
        hero->setAmulet("없음");
    }

    void setName(const string& name) override {
        hero->setName(name);
    }

    void equipLeftHand(const string& item) override {
        hero->setLeftHand(item);
    }

    void equipRightHand(const string& item) override {
        hero->setRightHand(item);
    }

    void equipLeftRing(const string& item) override {
        hero->setLeftRing(item);
    }

    void equipRightRing(const string& item) override {
        hero->setRightRing(item);
    }

    void equipArmor(const string& item) override {
        hero->setArmor(item);
    }

    void equipAmulet(const string& item) override {
        hero->setAmulet(item);
    }

    unique_ptr<Hero> getResult() override {
        return std::move(hero);
    }
};

// -----------------------------
// Director: HeroCreationDirector
//  - 영웅 생성 레시피를 알고 있는 "조립 책임자" 역할
// -----------------------------
class HeroCreationDirector {
public:
    // 해머딘(블레스드 해머 팔라딘) 세팅
    unique_ptr<Hero> createHammerdin(HeroBuilder& builder) {
        builder.reset();
        builder.setName("해머딘");
        builder.equipLeftHand("스피릿 크리스탈 소드");
        builder.equipRightHand("스피릿 성기사 방패");
        builder.equipLeftRing("패캐 링");
        builder.equipRightRing("레번 프로스트");
        builder.equipArmor("수수께끼 아머");
        builder.equipAmulet("마라의 만화경");
        return builder.getResult();
    }

    // 블리자드 소서 세팅
    unique_ptr<Hero> createBlizzardSorceress(HeroBuilder& builder) {
        builder.reset();
        builder.setName("블리자드 소서");
        builder.equipLeftHand("오큘러스 스태프");
        builder.equipRightHand("모너크 스피릿 방패");
        builder.equipLeftRing("조던");
        builder.equipRightRing("조던");
        builder.equipArmor("우수한 재료의 탈-갑");
        builder.equipAmulet("탈 라샤 아뮬렛");
        return builder.getResult();
    }

    // 휠윈드 바바 세팅
    unique_ptr<Hero> createWhirlwindBarbarian(HeroBuilder& builder) {
        builder.reset();
        builder.setName("휠윈드 바바");
        builder.equipLeftHand("그리프언드의 쌍도끼");
        builder.equipRightHand("그리프언드의 쌍도끼");
        builder.equipLeftRing("흡혈 링");
        builder.equipRightRing("흡혈 링");
        builder.equipArmor("수수께끼 아머");
        builder.equipAmulet("하이로드의 분노");
        return builder.getResult();
    }
};
