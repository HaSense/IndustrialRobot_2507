#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Weapon {
protected:
    int level;
    string name;

public:
    //생성자
    Weapon(){
        level = 1;
        name = "무기";
    }

    virtual void attack(){
        cout << "무기로 공격하다!" << endl;
    }
    void showLevel() {
        cout << "level : " << level << endl;
    }
    void showName() {
        cout << "Name : " << name << endl;
    }
};
class Sword : public Weapon {
public:
    Sword(){
        level = 2;
        name = "칼";
    }
    void attack() override {
        cout << name <<"로 공격하다!" << endl;
    }
};
class Axe : public Weapon {
public:
    Axe(){
        level = 2;
        name = "도끼";
    }
    void attack() override {
        cout << name <<"로 공격하다!" << endl;
    }
};

int main()
{
    Weapon* weapon = new Weapon();
    weapon->showName();
    weapon->showLevel();
    weapon->attack();
    delete weapon;

    unique_ptr<Sword> sword = make_unique<Sword>();
    sword->showName();
    sword->showLevel();
    sword->attack();

    unique_ptr<Axe> axe = make_unique<Axe>();
    axe->showName();
    axe->showLevel();
    axe->attack();

    return 0;
}
