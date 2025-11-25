#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> vec;    //vector 선언
    vec.push_back(1);   //값 삽입
    vec.push_back(2);
    vec.push_back(3);

    vector<int>::iterator it;   //순환자 선언, 순환자는 포인터변수!!

    for(it=vec.begin(); it != vec.end(); it++){
        cout << *it << endl;
    }
    
    return 0;
}
