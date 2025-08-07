#include <iostream>
#include <string>

using namespace std;

class Horse
{
public:
    void run(){ 
        cout << "달리다~~~!!!" << endl;
    }
};

class Bird
{
public:
    void fly()
    {
        cout << "날아 다닌다." << endl;
    }
};

//목표 클래스
class Unicorn :public Horse, public Bird
{
};

int main()
{
    Unicorn uni;
    uni.fly();
    uni.run();

    //Unicorn uni2{}; //c++ stack에 객체 생성
    //Unicorn uni3 = {};   //c++11 표현 변수초기화 0, 0
    //Unicorn* uni4 = new Unicorn();

    //Unicorn uni();  //함수형 (X)
    
    return 0;
}
