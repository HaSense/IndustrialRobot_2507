#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <list>

using namespace std;
int main()
{
    list<int>   intList;

    //1 ~10까지 입력하고 싶어!!!.
    for (int i = 0; i < 10; i++) {
        intList.push_back(i);
    }
    //5를 찾아서 제거하고 싶다.
    intList.remove(5);

    list<int>::iterator it; //순환자
    for (it = intList.begin(); it != intList.end(); it++) {
        cout << *it << endl;
    }

    return 0;
}
