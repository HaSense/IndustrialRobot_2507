#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
using namespace std;

class Person {
private:
    int id;
    string name;
    string pnumber;

public:
    Person(int id, string name, string pnumber):id(id),name(name),pnumber(pnumber) {}

    int getId(){
        return this->id;
    }
    string getName() {
        return this->name;
    }
    string getPnumber() {
        return this->pnumber;
    }

};

int main()
{
    vector<Person> addressbook;
    unique_ptr<Person> p = make_unique<Person>(1, "홍길동", "010-1111-1111");
    addressbook.push_back(*p);
    p = make_unique<Person>(2, "이순신", "010-2222-2222");
    addressbook.push_back(*p);

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
        
        int id=0;
        string name="";
        string pnumber="";
        
        switch (select)
        {
            case 1:
                //주소록 조회 코드를 작성합니다.
                for(Person p : addressbook){
                    cout << "ID : " << p.getId() << " / ";
                    cout << "이름 : " << p.getName() <<" / ";
                    cout << "전화번호 : " << p.getPnumber() << endl;
                }
                break;
            case 2:
            {
                //주소록 추가 코드를 작성합니다.
                int id=0;
                string name="";
                string pnumber="";

                cout << "새 ID 입력 : " ;
                cin >> id;
                cout << "새 아름 입력 : " ;
                cin >> name;
                cout << "새 전화번호 입력 : " ;
                cin >> pnumber;
                
                auto p = make_unique<Person>(id, name, pnumber);
                addressbook.push_back(*p);
                break;
            }                
            case 5:
            {
                exit(1); 
                break;
            }
                
            default :
                cout << "잘못된 번호 입력, 다시 입력해 주세요." << endl;
                break;
        }
    
    } while (select != 0);
    
}
