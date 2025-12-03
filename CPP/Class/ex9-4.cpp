#include <iostream>
#include <memory>
using namespace std;

class Shape{
public:
    virtual void draw() {
        cout << "도형을 그리다" << endl;
    }
};
class Circle : public Shape{
public:
    void draw() override {
        cout << "원을 그리다" << endl;
    }
};

int main()
{
    Shape* s = new Circle();
    s->draw();
    s->Shape::draw();   //범위지정 연산자를 활용하여 부모클래스의 함수 호출

    delete s;
    return 0;
}
