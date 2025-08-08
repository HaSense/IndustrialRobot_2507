#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
int main()
{
    vector<int> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    /*for (int i = 0; i < 3; i++) {
        cout << vec[i] << endl;
    }*/

    for (int a : vec) {
        cout << a << endl;
    }

    return 0;
}








