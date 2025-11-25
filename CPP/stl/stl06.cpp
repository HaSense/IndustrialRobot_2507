#include <iostream>
#include <map>
using namespace std;

int main()
{
    map<string, string> dic;

    dic.insert(make_pair("love", "사랑")); //함수로 삽입
    //dic["love"] = "사랑"; //키에 값을 직접 삽입

    string kor = dic["love"]; //키를 준다
    string kor2 = dic.at("love"); //key를 주면 값을 kor2로 보낸다.
    
    cout << kor << endl; //value가 출력된다.
    cout << kor2 << endl; //value가 출력

    return 0;
}
