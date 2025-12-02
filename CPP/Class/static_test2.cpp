#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class Human {
public:
    void run() {
        cout << "달리다~!!!" << endl;
    }
    static void run2() {
        cout << "run2 달리다~!!!" << endl;
    }
};

void run() {
    cout << "달리다~!!!" << endl;
}

int main()
{
    //run();
    Human::run2(); //static
    
    Human human;
    human.run();
    

    return 0;
}
