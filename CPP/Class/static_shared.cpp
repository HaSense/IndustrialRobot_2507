#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Person
{
public:
    int money;
    void addMoney(int money)
    {
        this->money += money;
    }

    //static
    static inline int shareMoney = 10;
    static void addShared(int n){
        shareMoney += n;
    } 
};

int main()
{
    Person::addShared(50);
    cout << "Shared money: " << Person::shareMoney << endl; //60
    
    Person han;
    han.money = 100;
    han.shareMoney = 200;
    cout << "Han's money: " << han.money << endl; //100
    cout << "Han's shared money: " << han.shareMoney << endl; //200
    

    Person::shareMoney = 300; //300
    han.addShared(50); //350
    Person::addShared(100); //450

    cout << han.money << " : " << han.shareMoney << endl; // 100 : 450
    return 0;
}



