#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

int main()
{
    string s1 = "Hello";
    string s2 = "Hello";

    const char* p1 = "Hello";
    const char* p2 = "Hello";

    cout << "s1 변수의 주소 : " << &s1 << endl;
    cout << "s2 변수의 주소 : " << &s2 << endl << endl;

    cout << "s1.c_str : " << (void*)s1.c_str() << endl;
    cout << "s2.c_str : " << (void*)s2.c_str() << endl << endl;
    

    cout << "p1 주소 : " << (void*)p1 << endl;
    cout << "p2 주소 : " << (void*)p2 << endl;



    return 0;
}







