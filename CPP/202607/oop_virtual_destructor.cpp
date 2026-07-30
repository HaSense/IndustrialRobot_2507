#include <iostream>
using namespace std;

class Base{
public:
    Base(){
        cout << "Base 생성자" << endl;
    }
    virtual ~Base(){
        cout << "Base 소멸자" << endl;
    }
};
class Derived : public Base{
public:
    Derived(){
        cout << "Derived 생성자" << endl;
    }
    virtual ~Derived(){
        cout << "Derived 소멸자" << endl;
    }
};

int main()
{
    // Derived* dp = new Derived();
    Base* bp = new Derived();

    // delete dp;
    delete bp;

    return 0;
}
