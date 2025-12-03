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
class Rect : public Shape{
public:
    void draw() override {
        cout << "사각형을 그리다" << endl;
    }
};
class Line : public Shape{
public:
    void draw() override {
        cout << "선을 그리다" << endl;
    }
};
//전역함수
void paint(Shape* p){
    p->draw();
    delete p; // 동적할당된 메모리 해제
}

int main()
{
    paint(new Circle());
    paint(new Rect());
    paint(new Line());
    
    return 0;
}
