#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class A{
public:
    // A() {
    //     cout << "디폴트 생성자 A" << endl;
    // };
    A(int x) {
        cout << "인자가 정수 1개있는 생성자 A" << endl;
    }
};
class B : public A {
public:
    // B() {
    //     cout << "디폴트 생성자 B" << endl;
    // };
    B(int x) : A(x){        //관심있게 볼 부분!
        cout << "인자가 정수 1개있는 생성자 B" << endl;
    }
};
int main()
{
    B b(5);
    return 0;
}
