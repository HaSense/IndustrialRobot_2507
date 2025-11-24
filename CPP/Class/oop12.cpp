#include <iostream>
#include <cstdlib>
using namespace std;

class Person {};
class Student : public Person{};
class StudentWorker : public Student{};

int main()
{
    StudentWorker gildong;
    
    return 0;
}
