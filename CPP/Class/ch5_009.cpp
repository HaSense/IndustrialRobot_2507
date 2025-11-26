#include <iostream>
using namespace std;

class Circle{
public:
    void getArea(){
        cout <<"넓이 구하는 함수" << endl;
    }
};
int main()
{
    Circle circle; //객체 stack에 존재
    Circle& ref = circle; //ref레퍼런스  , *ref ->객체

    ref.getArea();
    
    return 0;
}
