#include <iostream>
using namespace std;

class Shape {
public:
    int lineThick;
    Shape() {
        lineThick = 1;
    }
    virtual void draw() {
        cout << "도형을 그리다." << endl;
    }
};
class Circle : public Shape {
public:
    void draw() override {
        cout << "원을 그리다." << endl;
    }
};
class Triangle : public Shape {
public:    
    void draw() override {
        cout << "삼각형을 그리다." << endl;
    }
};
int main()
{
    Shape shape;
    shape.draw();

    Circle circle;
    circle.draw();
    Triangle tri;
    tri.draw();

    return 0;
}
