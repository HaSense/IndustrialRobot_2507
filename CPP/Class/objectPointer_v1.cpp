#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Circle
{
public:
    int radius;
    double getArea() {
        return 3.14 * radius * radius;
    }
};

int main()
{   
    Circle donut;
    cout << "donut : 넓이 : " << donut.getArea() << endl;

    Circle* p;
    p = &donut; //stack
    cout << "p: 넓이 : " << p->getArea() << endl;
    cout << "p: 넓이 : " << (*p).getArea() << endl;

    Circle* donut2 = new Circle(); //heap
    cout << "donut2 넓이 : " << donut2->getArea() << endl;
    cout << "donut2 넓이 : " << (*donut2).getArea() << endl;
    delete donut2;
    
    return 0;
}
