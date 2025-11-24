#include <iostream>
using namespace std;

template <class T>
void mySwap(T &a, T &b)
{
    T tmp;
    tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    char a='A', b = 'B';
    mySwap(a, b);

    cout << "a: " << a << "  b: " << b << endl; 
    return 0;
}
