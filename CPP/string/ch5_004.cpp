#include <iostream>
#include <string>

using namespace std;

int main()
{
    char* chs = "12345";
    string str = "12345";
    char number[100] = "12345";

    cout << "chs : " << stoi(chs) << endl;
    cout << "str : " << stoi(str) << endl;
    cout << "number[100] : " << stoi(number) << endl;
    
    return 0;
}
