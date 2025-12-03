#include <iostream>
#include <memory>
using namespace std;

class Parent{
public:
    virtual void f(){
        cout << "Parent::f() called." << endl;
    }
};
class Child : public Parent {
public:
    void f() override{
        cout << "Child::f() called." << endl;
    }   
};

int main()
{
    unique_ptr<Child> child = make_unique<Child>();
    child->f();

    unique_ptr<Parent> parent = make_unique<Child>();
    parent->f();


    return 0;
}
