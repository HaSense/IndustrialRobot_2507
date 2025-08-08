#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
int main()
{
    vector<char> vec;       //list, deque, stack, map ...

    vec.push_back('A');
    vec.push_back('B');
    vec.push_back('C');

    /*vec.pop_back();
    vec.pop_back();*/
    vec.clear();

    /*for (int i = 0; i < 2; i++) {
        cout << vec[i] << endl;
    }*/

    for (char a : vec) {
        cout << a << endl;
    }

    return 0;
}








