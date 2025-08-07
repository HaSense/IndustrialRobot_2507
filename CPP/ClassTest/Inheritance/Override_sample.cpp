#include <iostream>
#include <vector>
using namespace std;

class Shape
{
public:
    virtual void Draw() = 0;
    virtual ~Shape()
    {
    };
};
class Rectangle : public Shape
{
public:
    void Draw() {
        cout << "사각형을 그리다." << endl;
    }
};
class Circle : public Shape
{
public:
    void Draw() {
        cout << "원을 그리다." << endl;
    }
};
class Triangle : public Shape
{
public:
    void Draw() {
        cout << "삼각형을 그리다." << endl;
    }
};


int main()
{
    vector<Shape*> shapes;
    Rectangle rect;
    Circle circle;
    Triangle triangle;

    shapes.push_back(&rect);
    shapes.push_back(&circle);
    shapes.push_back(&triangle);

    for (Shape* s : shapes) {
        s->Draw();
    }
    return 0;
}

/*
사각형을 그리다.
원을 그리다.
삼각형을 그리다.
*/




