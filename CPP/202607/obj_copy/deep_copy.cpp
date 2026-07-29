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

    // 깊은 복사
    Number(const Number& other)
    {
        value = new int(*other.value);
    }

    ~Number()
    {
        delete value;
    }
};

int main()
{
    Number n1(10);

    Number n2 = n1;

    *n2.value = 100;

    cout << "n1 : " << *n1.value << endl;
    cout << "n2 : " << *n2.value << endl;
}
