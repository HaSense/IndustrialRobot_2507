#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace std;

class Hero{
public:
    virtual void attack() {
        cout << "기본 공격 !!!" << endl;
    }
};
class Knight : public Hero {
public:
    void attack() override{
        cout << "기사 공격 !!!" << endl;
    }
};
class Wizard : public Hero {
public:
    void attack() override{
        cout << "마법사 공격 !!!" << endl;
    }
};

int main()
{
    Hero* list[3];
    list[0] = new Hero();
    list[1] = new Knight();
    list[2] = new Wizard();

    //vector<Hero*> list; //Hero[0], Hero[1], Hero[1]
    // list.push_back(new Hero());
    // list.push_back(new Knight());
    // list.push_back(new Wizard());
    for(Hero* h : list){
        h->attack();
        delete h;
    }

    // for(int i=0; i<3; i++){
    //     list[i]->attack();
    //     delete list[i];
    // }
    return 0;
}
