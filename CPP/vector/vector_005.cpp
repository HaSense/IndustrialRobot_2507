#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
int main()
{
    vector<int> vec;

    for (int i = 0; i < 10; i++) {
        vec.push_back(i);
    }
    //vector에서 요소 지우기 , remove없음. erase(index)
    vec.erase(vec.begin() + 1); // 1을 삭제

    //순환자
    vector<int>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++) {
        cout << *it << endl;
    }


    return 0;
}







