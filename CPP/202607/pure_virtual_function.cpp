#include <iostream>
using namespace std;

class Shape{    //순수 가상 함수가 1개라도 있으면 추상클래스가 된다.
public:
    virtual void draw() = 0; //순수 가상 함수
};
class Circle : public Shape{
public:
    virtual void draw() override{
        cout << "Circle";
    }    
};

int main()
{
    //Shape shape;
    Circle circle;

    return 0;
}
