#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<string> vec;
    string name;
    for(int i=0; i<5; i++){
        cout << i+1 <<">>";
        getline(cin, name);
        vec.push_back(name); //벡터 메모리(요소)추가
    }

    sort(vec.begin(), vec.end());

    for(string s : vec){
        cout << s <<"    ";
    }
    cout<<endl;

    
    return 0;
}
