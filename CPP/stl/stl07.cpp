#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> v = {2,3,4,5,1}; //벡터 선언과 동시 초기화

    //sort(v.begin(), v.end());
    //reverse(v.begin(), v.end()); //1. 내림차순 - 가장 쉬움
    sort(v.begin(), v.end(), 
        [](int a, int b){       //람다 표현식
            return a > b;
        }
    );
    
    // vector<int>::iterator it;
    // for(it=v.begin(); it!=v.end(); it++){
    //     cout << *it << "\t";
    // }
    // cout<<endl;

    for(int i : v){
        cout << i << "\t";
    }
    cout<<endl;
    
    return 0;
}
