#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

class Knight
{
};
class Wizard
{
public:
    int level;
    string name;

    Wizard() {};
    Wizard(int level, string name) {
        this->level = level;
        this->name = name;
    }
};
int main()
{
    Knight tom; //객체 생성 Stack
    Knight* tom2 = new Knight(); //객체 생성 Heap
    delete tom2;
    //smart Pointer
    unique_ptr<Knight> tom3 = make_unique<Knight>();
    unique_ptr<Wizard> mary = make_unique<Wizard>();
    unique_ptr<Wizard> mary2 = make_unique<Wizard>(20, "메리2");

    cout << mary2.get()->name << endl;
    cout << mary2.get()->level << endl;

    return 0;
}



