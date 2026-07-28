/*
  1. 위임생성자, 타겟생성자 문법

  2. 객체를 스택메모리에 생성
     객체를 힙메모리에 생성
     객체를 스마트포인터 문법을 사용하여 생성 자동으로 힙에서 지워줍니다.
  */

#include <iostream>
#include <memory>
using namespace std;

class Circle{
public:
    int radius;

    Circle():Circle(1){}; //디폴트 생성자 -> 위임생성자 문법을 통해 만들었다.
    Circle(int r){ //인자가 있는 생성자 -> 타겟생성자
        radius = r;
        cout << "반지름 " << radius << " 원 생성" << endl;
    };
};

int main()
{
    Circle circle;      //객체를 stack 메모리에 생성
    Circle circle2(30);
    
    Circle* circle3 = new Circle(); //객체를 heap 메모리에 생성
    Circle* circle4 = new Circle(30);
    delete circle3;
    delete circle4;

    unique_ptr<Circle> circle5 = make_unique<Circle>();
    unique_ptr<Circle> circle6 = make_unique<Circle>(30);

    

    return 0;
}
