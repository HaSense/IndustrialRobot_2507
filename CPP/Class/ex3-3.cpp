#include <iostream>

using namespace std;
class Circle
{
public:
    //1.멤버 변수
    int radius;
    string color;
    //2.생성자
    Circle(){//default 생성자
        radius = 1;
        cout << "반지름 : " << radius << endl;
    } 
    Circle(int r) {
        radius = r;
        cout << "반지름 : " << radius << endl;
    }
    Circle(int r, string c) {
        radius = r;
        color = c;
        cout << "반지름 : " << radius << " 색상 : " << color << endl;
    }
    //3.멤버메소드
};
int main()
{
    Circle donut;
    Circle pizza(30);
    Circle tire(10, "RED");

    return 0;
}
