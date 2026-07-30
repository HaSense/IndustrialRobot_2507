/*
 1. 상속
 2. 가상함수, 오버라이딩
  */

#include <iostream>
using namespace std;

class Phone{
public:
    virtual void call(){
        cout << "전화를 걸다." << endl;
    }
};
class MobilePhone : public Phone{
public:
    void call() override{
        cout <<"모바일폰으로 전화를 걸다." << endl;
    }
};
class MusicPhone : public MobilePhone{
public:
    void call() override{
        cout <<"뮤직폰으로 전화를 걸다." << endl;
    }
};

int main()
{
    // MusicPhone music_phone;
    // music_phone.call();

    Phone* phone = new MusicPhone();
    phone->call();

    delete phone;

    return 0;
}
