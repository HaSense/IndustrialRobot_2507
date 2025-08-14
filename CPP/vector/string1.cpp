#include <iostream>
#include <string>

using namespace std;
int main()
{
    string s1 = "Hello";
    string s2 = " World!";
    string s4 = "Hello";
    string s5 = "I love you!";

    string s3 = s1 + s2;
    //string s4 = s1 + 100; //Error
    
    cout << "s1.size() : " << s1.size() << endl;
    cout << "s2.size() : " << s2.size() << endl;
    s1.clear();
    cout << "s1.empty() : " << s1.empty() << endl;
    cout << "s2.find('d') : " << s2.find('d') << endl;
    cout << "s5.find(\"love\") : " << s5.find("love") << endl;

    string s6 = "901225-1120337";
    cout << s6.substr(2, 4) << endl;


    return 0;
}
