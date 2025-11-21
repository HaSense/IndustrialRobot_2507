#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    int n;
    int* p;
    int* p2;

    p = &n;     //stack

    p2 = new int;   //heap
    delete p2;

    return 0;
}
