#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Student {
private:
    string name;
    int age;
    string major;
    int id;
private:
    //생성자
    Student(string name, int age, string major, int id)
        : name(name), age(age), major(major), id(id) {}
public:

    class Builder {
    private:
        string name = "이름";
        int age = 1;
        string major = "컴퓨터공학";
        int id = 0;
    public:
        Builder& setName(const string& name) {
            this->name = name;
            return *this;
        }   
        Builder& setAge(int age) {
            this->age = age;
            return *this;   
        }
        Builder& setMajor(const string& major) {
            this->major = major;
            return *this;
        }
        Builder& setId(int id) {
            this->id = id;
            return *this;   
        }
        
        Student build() {
            return Student(name, age, major, id);
        }
    };//end of Builder
    //getter
    string getName() { 
        return name; 
    }
    int getAge() { 
        return age; 
    }
    string getMajor() { 
        return major; 
    }
    int getId() { 
        return id;
    }
     
};
int main()
{
    //Builder를 이용하여 Student 객체 생성
    Student student = Student::Builder()
                        .setName("홍길동")
                        .setAge(20)
                        .setMajor("소프트웨어공학")
                        .setId(2023001)
                        .build();

    //Student 정보 출력
    cout << "Name: " << student.getName() << endl;
    cout << "Age: " << student.getAge() << endl;
    cout << "Major: " << student.getMajor() << endl;
    cout << "ID: " << student.getId() << endl;
    return 0;
}
