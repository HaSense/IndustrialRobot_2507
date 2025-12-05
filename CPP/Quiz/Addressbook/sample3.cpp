#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class Person
{
private:
    int id;
    string name;
    string pnumber;
public:
    Person():id(0), name(""), pnumber(""){
    }
    Person(int id, string name, string pnumber)
        :id(id), name(name), pnumber(pnumber) {

    }
    //Getter
    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    string getPnumber(){
        return pnumber;
    }
    //Setter
    void setNo(int id){
        this->id = id;
    }
    void setName(string name){
        this->name = name;
    }
    void setPhoneNumber(string pnumber){
        this->pnumber = pnumber;
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
    
        switch (select)
        {
            case 1:
                //주소록 조회 코드를 작성합니다.
                for(Person p : addressbook){
                    cout << "ID : " <<  p.getId() << " / ";
                    cout << "Name : " <<  p.getName() << " / ";
                    cout << "PNumber : " <<  p.getPnumber() << endl;
                }
                break;

            case 2:
                //주소록 추가 코드를 작성합니다.
                {
                    int id=0;
                    string name="";
                    string pnumber="";
                    
                    cout <<"id 입력 : " ;
                    cin >> id;
                    cout <<"name 입력 : ";
                    cin >> name;
                    cout <<"pnumber 입력 :";
                    cin >> pnumber;
                    auto person = make_unique<Person>(id, name, pnumber);
                    addressbook.push_back(*person);
                }
                break;

            case 3:
                // 주소록 수정
                {
                    int id;
                    cout << "수정할 사람의 ID 입력 : ";
                    cin >> id;

                    // Person 이 const 가 아니므로 람다 파라미터도 Person& 로 받는다.
                    auto it = find_if(addressbook.begin(), addressbook.end(),
                                      [id](Person& person){
                                          return person.getId() == id;
                                      });

                    if (it == addressbook.end()) {
                        cout << "ID " << id << " 에 해당하는 주소록 정보가 없습니다." << endl;
                    } else {
                        string newName;
                        string newPnumber;

                        cout << "새 이름 입력 (현재: " << it->getName() << ") : ";
                        cin >> newName;
                        cout << "새 전화번호 입력 (현재: " << it->getPnumber() << ") : ";
                        cin >> newPnumber;

                        it->setName(newName);
                        it->setPhoneNumber(newPnumber);

                        cout << "주소록 정보가 수정되었습니다." << endl;
                    }
                }
                break;

            case 4:
                // 주소록 삭제
                {
                    int id;
                    cout << "삭제할 사람의 ID 입력 : ";
                    cin >> id;

                    auto it = find_if(addressbook.begin(), addressbook.end(),
                                      [id](Person& person){
                                          return person.getId() == id;
                                      });

                    if (it == addressbook.end()) {
                        cout << "ID " << id << " 에 해당하는 주소록 정보가 없습니다." << endl;
                    } else {
                        addressbook.erase(it);
                        cout << "주소록 정보가 삭제되었습니다." << endl;
                    }
                }
                break;

            case 5:
                exit(1); 
                break;
        }
    
    } while (select != 0);
}
