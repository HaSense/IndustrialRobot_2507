#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Calculator {
private:
    int a;
    int b;
public:
    void inputTwoNumbers(){
        int a, b;
        cout << "두 수를 입력하세요 : ";
        cin >> a >> b;
        this->a = a;
        this->b = b;
    }
    int plus() {
        return a + b;
    }

};

int main()
{
    unique_ptr<Calculator> cal = make_unique<Calculator>(); //heap
    cal->inputTwoNumbers();
    cout << "plus : " << cal->plus() << endl;

    return 0;
}
