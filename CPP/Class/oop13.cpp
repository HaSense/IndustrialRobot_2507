#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Person{
};
class Student : public Person {
};

int main()
{
    Person father; //stack - delete(x)
    //c++ 11
    unique_ptr<Person> gildong = make_unique<Person>(); //heap객체생성 delete(x)
    
    Person* gildong2 = new Student();
    delete gildong2;


    return 0;
}
