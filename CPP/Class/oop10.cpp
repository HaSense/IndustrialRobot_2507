#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Circle
{
public:
    //멤버 변수
    int radius;
    //생성자
    Circle(int r){
        radius = r;
    }
    //멤버 함수
    void draw() {
        cout << "원을 그리다." << endl;
    }
};

int main()
{
    //기본타입 동적할당
    int* p = new int;     //bool, short, float, double ...
    *p = 5;
    delete p;
    
    //배열 동적할당
    int* p2 = new int[3];
    p2[0] = 0;
    p2[1] = 1;
    delete [] p;

    //c++11 스마트포인터   <Class Type>
    unique_ptr<int> p3 = make_unique<int>(5);
    std::unique_ptr<Circle> p4 = std::make_unique<Circle>(10);  
    p4->draw();

    return 0;
}
