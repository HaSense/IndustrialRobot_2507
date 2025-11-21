#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    int* p;
    p = new int; //heap 4byte 정수형 공간/변수 생성
    
    *p = 5; //stack 정수형 상수 5값을 대입 --> heap
    cout << "*p : " << *p << endl;

    int n = *p; //heap공간에 있는 5값을 stack 지역변수 n에 대입
    cout << "n : " << n << endl;

    delete p;
    return 0;
}
