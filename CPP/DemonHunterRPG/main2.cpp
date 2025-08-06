#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <thread>
#include <memory>
#include <chrono>

using namespace std;

class Wizard
{
protected:
    string name;
    int level;
    int mp;

public:
    Wizard() 
    {
        name = "마법사";
    }
    Wizard(string name, int level, int mp)
        :name(name), level(level), mp(mp)
    {
    }
    //멤버 함수
    virtual string Attack()
    {
        return "공격하다!!!";
    }

    string Attack(string weapon)
    {
        return weapon + "로 공격하다!!!";
    }

    string Attack(string weapon, string power)
    {
        return weapon + "로 " + power + "만큼 공격하다!!!";
    }

    //Getter, Setter
    string getName()
    {
        return name;
    }
    void setName(string name)
    {
        this->name = name;      //L:멤버가 객체가 되었을때 R:호출한 쪽에서 가져온값 
    }
    //level에 대한 getter, setter
    int getLevel()
    {
        return level;
    }
    void setLevel(int level)
    {
        this->level = level;
    }


};

class WhiteWizard : public Wizard
{
public:
    using Wizard::Attack;

    string Attack() override
    {
        return "마법으로 공격하다!!!";
    }
};

int main()
{
    Wizard Rui; //default 생성자로 만든 객체
    cout << Rui.getName() << endl;

    Wizard tamy("타미", 1, 50); //인자가 3개 있는 생성자로 만든 객체
    cout << tamy.getName() << endl;

    cout << Rui.Attack() << endl;
    cout << Rui.Attack("총") << endl;
    cout << Rui.Attack("칼", "80") << endl << endl;


    WhiteWizard leo;    //객체생성
    cout << leo.Attack() << endl;
    cout << leo.Attack("총") << endl;
    cout << leo.Attack("칼", "80") << endl;

    return 0;
}



