#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

class Shape
{
public:
    virtual void Draw() {
        cout << "도형을 그리다." << endl;
    }
};
class Triangle : public Shape
{
public:
    void Draw() override {
        cout << "삼각형을 그리다." << endl;
    }
};
class Rectangle : public Shape
{
public:
    void Draw() override {
        cout << "사각형을 그리다." << endl;
    }
};
class Circle : public Shape
{
public:
    void Draw() override {
        cout << "원을 그리다." << endl;
    }

};

int main()
{
    Shape s;
    s.Draw();

    Triangle tri;
    tri.Draw();
    Rectangle rec;
    rec.Draw();
    Circle cir;
    cir.Draw();

    return 0;
}
