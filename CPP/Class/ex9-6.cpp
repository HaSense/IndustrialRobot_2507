#include <iostream>
#include <memory>
using namespace std;

class Parent
{
public:
    Parent() {
        cout << "부모 생성자 호출 " << endl;
    }
    virtual ~Parent() {
        cout << "부모 소멸자 호출 " << endl;
    }
};
class Child : public Parent
{
public:
    Child() {
        cout << "자식 생성자 호출 " << endl;
    }   
    virtual ~Child() {
        cout << "자식 소멸자 호출 " << endl;
    }
};

int main()
{
    //Parent p;
    //Child c;
    
    // Parent* p2 = new Parent();
    // delete p2;
    
    // Child* c2 = new Child();
    // delete c2;

    Parent* p3 = new Child();
    delete p3;

    return 0;
}




