#include <iostream>
using namespace std;

class Horse {
public:
    void run() {
        cout << "달리다~!!" << endl;
    }
    void eat() {

    }
};
class Bird {
public:
    void fly() {
        cout << "날다~~!!" << endl;
    }
    void eat(){

    }
};

class Unicorn :public Horse, public Bird{
};
int main()
{
    Unicorn mary;

    mary.run();
    mary.fly();
    mary.Horse::eat();
    mary.Bird::eat();    
    return 0;
}
