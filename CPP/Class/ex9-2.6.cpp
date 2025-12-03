#include <iostream>
#include <memory>
using namespace std;

class Shape{  //abstract 키워드 없음
public:
    virtual void draw()=0; //순수 가상 함수
    // virtual void draw() {
    //     cout << "도형을 그리다" << endl;
    // }
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

    //Shape s;    //추상 클래스는 객체 생성 불가
    //s.draw();

    Circle c;
    c.draw();
    Shape* ps = &c; //추상 클래스형 포인터는 가능
    ps->draw();
    
    return 0;
}
