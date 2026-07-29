#include <iostream>
using namespace std;

class Number
{
public:
    int number;
    int* value;

    Number(int n)
        : number(n), value(new int(n))
    {
    }

    // 복사 생성자를 직접 작성하지 않음
    // 컴파일러가 다음과 비슷한 동작을 자동 생성함:
    //
    // Number(const Number& other)
    //     : number(other.number),
    //       value(other.value)
    // {
    // }
};

int main()
{
    Number n1(10);
    Number n2 = n1;  // 컴파일러 생성 복사 생성자 호출

    cout << "n1.number: " << n1.number << '\n';
    cout << "n2.number: " << n2.number << '\n';

    cout << "n1.value 주소: " << n1.value << '\n';
    cout << "n2.value 주소: " << n2.value << '\n';

    *n2.value = 100;

    cout << "n1이 가리키는 값: " << *n1.value << '\n';
    cout << "n2가 가리키는 값: " << *n2.value << '\n';

    delete n1.value;  // 공유된 메모리이므로 한 번만 삭제
}
