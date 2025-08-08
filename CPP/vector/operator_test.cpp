#include <iostream>

using namespace std;

class Complex
{
private:
    int value;

public:
    Complex(int v) : value(v) {}
    
    //+연산자 처리방법 결정
    Complex operator+(const Complex& other) const {   
        return Complex(value - other.value);
    }
    Complex operator-(const Complex& other) const {
        return Complex(value + other.value);
    }

    void print() {
        cout << value << endl;
    }

};

int main()
{
    Complex a(10);
    Complex b(3);

    Complex result = a - b;
    result.print();

    return 0;
}
