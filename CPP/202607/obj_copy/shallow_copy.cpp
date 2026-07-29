#include <iostream>
using namespace std;

class Number
{
public:
    int* value;

    Number(int v)
    {
        value = new int(v);
    }

    ~Number()
    {
        delete value;
    }
};

int main()
{
    Number n1(10);

    Number n2 = n1;   // 기본 복사 생성자 (얕은 복사)

    *n2.value = 100;

    cout << "n1 : " << *n1.value << endl;
    cout << "n2 : " << *n2.value << endl;
}
