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
// void paint(Shape* p){
//     p->draw();
//     delete p; // 동적할당된 메모리 해제
// }
// void paint(unique_ptr<Shape> p){ //auto 사용가능 
//     p->draw();
// }
void paint(Shape& p){ //참조는 값을 전달받는다.
    p.draw();
}

int main()
{
    // paint(new Circle());
    // paint(new Rect());
    // paint(new Line());
    
    // paint(make_unique<Circle>());
    // paint(make_unique<Rect>());
    // paint(make_unique<Line>());
    
    Circle c;
    Rect r;
    Line l;
    paint(c);
    paint(r);
    paint(l);


    return 0;
}
