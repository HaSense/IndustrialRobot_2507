#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <thread>
#include <memory>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Story
{
public:
    void DelayPrint(const string& text, int dealyTime = 50) {
        for (char ch : text) {
            cout << ch << flush;
            this_thread::sleep_for(milliseconds(dealyTime));
        }
    }
};

class Knight
{
protected:
    string name;
    int level;
    int hp;
    int mp;
public:
    virtual string Attack() {
        return "공격하다!!!";
    }
    //virtual string Attack() = 0; //순수가상함수
    virtual string Attack(string weapon) {
        return weapon + "로 공격하다!!!";
    }
    //Getter, Setter
    void setLevel(int _level){
        level = _level;
    }
    int getLevel() {
        return level;
    }
 
};
class DarkKnight : public Knight
{
public:
    string Attack() override {
        return "암흑기사가 공격하다!!!";
    }
    string Attack(string weapon) override{
        return weapon + "로 공격하다!!!";
    }
};
int main()
{
    Story story;
    story.DelayPrint("때는 BC. 5700년전 대륙은 어둠으로 물들고 적들은 왕성해 졌다.", 50);
    story.DelayPrint("정의를 이루기 위한 영웅이 탄생할 때가 되었다.", 50);


    //Knight* p = new Knight();

    DarkKnight tomy;
    tomy.setLevel(30);

    cout << tomy.Attack() << endl;
    cout << "레벨 : " << tomy.getLevel() << endl;

    cout << tomy.Attack("도끼") << endl;

    return 0;
}









