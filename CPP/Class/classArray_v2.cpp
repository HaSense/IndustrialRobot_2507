#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Circle
{
public:
    int radius;
    Circle() { 
        radius = 1;
    }
    Circle(int r){
        radius = r;
    }
    void setRadius(int r) {
        radius = r;
    }
    double getArea() {
        return 3.14 * radius * radius;
    }
};

int main()
{
    Circle cir[3];  //object stack
    cir[0].setRadius(10);
    cout <<"area : " << cir[0].getArea() << endl;

    Circle* p;
    p = cir;
    cout << "area : " << p[0].getArea() << endl;

    Circle* cir2[3]; //object heap
    cir2[0] = new Circle(); //배열요소에 객체 생성이 필수
    cir2[0]->setRadius(10);
    cout <<"area : " << cir2[0]->getArea() << endl;

    delete cir2[0];

    return 0;
}
