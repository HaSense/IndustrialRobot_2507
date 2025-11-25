#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <algorithm> //reverse
#include <cstring>
using namespace std;

int main()
{
    string str = "Hello World!";
    string str2;
    //getline(cin, str); //Hello World!;
    //문자열 string 복사
    str2.resize(str.size()); //문자열 size 맞추기
    copy(str.begin(), str.end(), str2.begin()); //stl copy를 사용가능
    
    //문자열 뒤짚기
    reverse(str.begin(), str.end());    
    cout << str << endl;

    //2. 고전 방법
    int length = str2.size();
    for(int i = length - 1; i>=0; i--){
        //cout << str2[i];
        cout << str2.at(i);
    }
    cout << endl;

    return 0;
}
