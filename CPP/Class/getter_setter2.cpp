#include <iostream>
using namespace std;

class Person
{
private:
    int stNumber;
    string name;

public:
    Person() {}
    Person(int _stNumber, string _name) {
        stNumber = _stNumber;
        name = _name;
    }
    //Getter, Setter
    void setStNumber(int _stNumber){
        stNumber = _stNumber;
    }
    int getStNumber(){
        return stNumber;
    }
    void setName(string _name){
        name = _name;
    }
    string getName() {
        return name; 
    }
};

int main()
{
    Person sam(1, "샘");
    cout << "시민번호 : " << sam.getStNumber() << endl; 
    cout << "이름 : " << sam.getName() << endl;

    Person jane;
    jane.setName("jane");
    jane.setStNumber(2);
    cout << "시민번호 : " << jane.getStNumber() << endl; 
    cout << "이름 : " << jane.getName() << endl;
    return 0;
}
