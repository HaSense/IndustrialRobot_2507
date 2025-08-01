#include <iostream>

using namespace std;

class Point
{
public:
    int x, y;
    //default 생성자
    Point() {
        x = 0;
        y = 0;
    }
    //인자가 2개 있는 생성자.
    Point(int initX, int initY) {
        this->x = initX;
        this->y = initY;
    }
    //복사 생성자
    Point(const Point& pt)
    {
        cout << "복사 생성자 호출됨" << endl;
        x = pt.x;
        y = pt.y;
    }

    //멤버 함수
    void Print()
    {
        cout << x << ", " << y << endl;
    }
};
int main()
{
    Point pt1;
    pt1.Print();

    Point* pt2 = new Point();
    pt2->Print();
    delete pt2;

    //인자가 있는 생성자로 생성
    Point pt3(3, 4);
    pt3.Print();

    Point* pt4 = new Point(3, 4);
    pt4->Print();
    delete pt4;

    
    Point pt5;
    pt5.Print();

    Point pt7;
    pt7 = pt5;  //객체끼리 대입 연산
    pt7.Print();

    Point pt8(pt5); //복사 생성자
    pt8.Print();
}












