#include <iostream>
using namespace std;

class Rect;//클래스 전방선언
class Manager;

class Manager {
public:
    bool equals(Rect r, Rect s); //구현을 미뤄서 동작.
};
class Rect{
private:
    int width;
    int height;
public:
    Rect(int width, int height) : width(width), height(height) {}
    friend bool Manager::equals(Rect r, Rect s); 
};
bool Manager::equals(Rect r, Rect s)
{
        if(r.width == s.width && r.height == s.height)
            return true;
        else
            return false;      
}

int main()
{
    Rect a(3, 4), b(3, 4);
    Manager mgr;

    if(mgr.equals(a, b))
        cout << "같음" << endl;
    else
        cout << "다름" << endl; 
    return 0;
}
