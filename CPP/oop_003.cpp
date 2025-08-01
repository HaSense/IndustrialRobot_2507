#include <iostream>
using namespace std;

class Calculator
{
public: 
    int a;
    int b;

    //생성자는 한개의 생성자도 없을 때는 자동으로 default 생성자를 
    //컴파일러가 만들어 줍니다.
    Calculator() {
        a = 0;
        b = 0;
    }
public:
    int Plus(int a, int b) {
        return a + b;
    }    
    int Minus(int a, int b) {
        return a - b;
    }
};

int main()
{
    Calculator cal;
    int x, y;
    cout << "첫번째 숫자를 입력하세요 : ";
    cin >> x;
    cout << "두번째 숫자를 입력하세요 : ";
    cin >> y;

    int result = cal.Plus(x, y);
    cout << "덧셈의 결과 : " << result << endl;
}










