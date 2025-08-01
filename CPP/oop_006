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

}










