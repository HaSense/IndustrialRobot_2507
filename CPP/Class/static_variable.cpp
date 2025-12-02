#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Person
{
public:
    int money = 0;
    static inline int sharedMoney = 0; //C++17부터 inline 키워드로 static 멤버 변수를 정의할 수 있음

    Person(){
        money++;
        sharedMoney++;
    }
};
//C++에서 static 멤버 변수에 정의는 필수
//int Person::sharedMoney = 10000; 


int main()
{
   Person p1;
   cout << p1.sharedMoney  << ":" << p1.money << endl;

   Person p2;
   cout << p2.sharedMoney  << ":" << p2.money << endl;

   Person p3;
   cout << p3.sharedMoney  << ":" << p3.money << endl;
 
    return 0;
}
