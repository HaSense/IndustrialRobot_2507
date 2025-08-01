#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

class Person
{
private:
    string name;
    int age;

public:
    Person() {};

    //Getter,Setter  간접접근, 직접접근
    void setName(string _name)
    {
        //전처리 암호화, 복호화, 
        name = _name;
        //후처리 eg..)로그 ...
    }
    string getName()
    {
        return name;
    }

    void setAge(int _age)
    {
        age = _age;
    }
    int getAge()
    {
        return age;
    }

private:  //내부 처리용
    void myFunction()
    {
        int result = age + 10;
    }

};

int main()
{
    Person hong;

    hong.setName("홍길동");

    cout << hong.getName() << endl;

    //hong.myFunction();

    
    
    return 0;
}
