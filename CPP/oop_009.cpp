#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

class Hero
{
protected:
    int level;
    string name;

    Hero() {
        level = 0;
        name = "영웅";

        cout << "Hero 생성자 호출" << endl;
    }
    ~Hero() {
        cout << "Hero 소멸자 호출" << endl;
    }

};

class Knight : public Hero
{
public:
    Knight() {
        level = 10;
        name = "기사";

        cout << "Knight 생성자 호출" << endl;
    }
    ~Knight() {
        cout << "Knight 소멸자 호출" << endl;
    }
};
class Wizard : public Hero
{

};


int main()
{
    Knight chals;

    
    return 0;
}
