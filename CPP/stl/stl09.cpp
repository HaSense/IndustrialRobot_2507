#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> vec = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    //1.혹시 원소들 값이 정렬되어 있지 않다면
    sort(vec.begin(), vec.end());
    //2.중복 찾기
    auto it_vec = unique(vec.begin(), vec.end());
    //3.중복 제거
    vec.erase(it_vec, vec.end());
    for(int i : vec){
        cout << i << " ";
    }
    cout << endl;
    cout << "vec size : " << vec.size() << endl;
    
    return 0;
}
