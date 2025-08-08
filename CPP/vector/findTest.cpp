#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;
int main()
{
    vector<int> vec = { 1, 2, 3, 4, 5 };
    //vector<int>::iterator it;

    auto it = find(vec.begin(), vec.end(), 7);

    if (it != vec.end())
        cout << "데이터가 있음(찾았음)" << endl;
    else
        cout << "데이터가 없음" << endl;

    return 0;
}




