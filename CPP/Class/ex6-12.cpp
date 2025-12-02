#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class PersonError
{
private:
    int money;
    static inline int staticMoney = 0; //C++17부터 static inline 멤버 변수 허용

public:
    static int getStaticMoney() { //static 함수에서는 static 멤버만 접근 가능
        //this->staticMoney = 100; //error
        return staticMoney;  //money 변수일 때(non-static변수일때) error
    }
    void setMoney(int money){    //non-static 함수에서는 static, non-static 멤버 모두 접근 가능
        this->money = money;
        staticMoney += money;
    }
    int getMoney(){
        return money;
    }
};

int main()
{
    int n = PersonError::getStaticMoney();

    PersonError pr;
    pr.setMoney(1000);
    cout << "Money: " << pr.getMoney() << endl;
    return 0;
}
