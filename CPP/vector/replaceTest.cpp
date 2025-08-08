#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<char> vec = { 'A','B','C'};

    replace(vec.begin(), vec.end(), 'B', 'Z');

    for (char ch : vec) {
        cout << ch << endl;
    }

    return 0;
}






