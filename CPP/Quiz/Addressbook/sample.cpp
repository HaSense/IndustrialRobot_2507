#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class Person
{
public:
    Person():no(0), name(""), phoneNumber(""){
    }
    Person(int no, string name, string phoneNumber)
        :no(no), name(name), phoneNumber(phoneNumber) {

    }
    //Getter
    int No(){
        return no;
    }
    string Name(){
        return name;
    }
    string PhoneNumber(){
        return phoneNumber;
    }
    //Setter
    void setNo(int no_){
        no = no_;
    }
    void setName(string name_){
        name = name_;
    }
    void setPhoneNumber(string phoneNumber_){
        phoneNumber = phoneNumber_;
    }
private:
    int no;
    string name;
    string phoneNumber;
};

int main()
{
    vector<Person> addressbook;

    int select = -1;
    do
    {
        cout << "---------------------------------------------------------------- -" << endl;
        cout << "1. 주소록 조회" << endl;
        cout << "2. 주소록 추가" << endl;
        cout << "3. 주소록 수정" << endl;
        cout << "4. 주소록 삭제" << endl;
        cout << "5. 종료" << endl;
        cout << " 메뉴 : ";
        cin >> select;
    
        switch (select)
        {
            case 1:
                //주소록 조회 코드를 작성합니다.
                break;
            case 2:
                //주소록 추가 코드를 작성합니다.
                break;
            case 5:
                exit(1); 
                break;
        }
    
    } while (select != 0);

}

