#include <iostream>
#include <cstdlib>
using namespace std;

class Person {
public:
    Person() {
        cout << "Person 생성자 호출" << endl;
    }
    ~Person() {
        cout << "Person 소멸자 호출" << endl;
    }
};

class Student : public Person {
public:
    Student() {
        cout << "Student 생성자 호출" << endl;
    }
    ~Student() {
        cout << "Student 소멸자 호출" << endl;
    }
};
class Researcher : public Person {
public:
    Researcher() {
        cout << "Researcher 생성자 호출" << endl;
    }
    ~Researcher() {
        cout << "Researcher 소멸자 호출" << endl;
    }
    
};

class StudentWorker : public Student{
public:
    StudentWorker(){
        cout << "StudentWorker 생성자 호출" << endl;
    }
    ~StudentWorker(){
        cout << "StudentWorker 소멸자 호출" << endl;
    }
};
class Professor : public Researcher{
public:
    Professor() {
        cout << "Professor 생성자 호출" << endl;
    }
    ~Professor() {
        cout << "Professor 소멸자 호출" << endl;
    }
};

int main()
{
    StudentWorker gilDong;
    Professor chulsu;
    
    return 0;
}
