#include <iostream>
#include <algorithm>

using namespace std;

class Dog 
{
//1.멤버 변수, 속성
public:         
    bool male;  
    int age;
    int weight;

//2.생성자 - 객체의 초기화와 관련이 있다.
public:
    Dog() {
        //디폴트 생성자
    }
    ~Dog() {
        //리소스 반환, 객체가 가지고 있는 자원이 있다면 이때 해제한다.
    }

//3.멤버 함수
public:         //protected, private
    void Bark() 
    {
        cout << "멍멍" << endl;
    }
    void Eat() {
        cout << "먹습니다." << endl;
    }
    void Sleep() {
        cout << "잠을 잡니다." << endl;
    }
};

int main()
{
    Dog zzong; //객체생성
    zzong.age = 5;
    zzong.male = true;
    zzong.weight = 7;

    cout << "쫑의 나이는 : " << zzong.age << "입니다." << endl;
    cout << "쫑의 몸무게는 : " << zzong.weight <<"입니다." << endl;
    zzong.Bark();
    zzong.Eat();
    zzong.Sleep();

    
}
