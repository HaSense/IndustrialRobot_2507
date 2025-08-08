#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

using namespace std;

class Person
{
private:
    int number;
    string name;
    string hp;

public:
    Person(int number, string name, string hp) :
        number(number), name(name), hp(hp) { }
    //Getter, Setter
    void setNumber(int number) {
        this->number = number;
    }
    int getNumber() {
        return this->number;
    }
    void setName(string name) {
        this->name = name;
    }
    string getName() {
        return this->name;
    }
    void setHp(string hp) {
        this->hp = hp;
    }
    string getHp() {
        return this->hp;
    }

};

int main()
{
    int input_num;
    string input_name;
    string input_hp;
    
    cout << "데이터의 번호를 입력해 주세요. : ";
    cin >> input_num;
    cout << "데이터의 이름을 입력해 주세요. : ";
    cin >> input_name;
    cout << "데이터의 전화번호를 입력해 주세요. : ";
    cin >> input_hp;

    Person person(input_num, input_name, input_hp);
    list<Person> addressbook;
   
    addressbook.push_back(person);
   

    for (Person p : addressbook) {
        cout << p.getNumber() << endl;
        cout << p.getName() << endl;
        cout << p.getHp() << endl;
    }
    
    return 0;
}
