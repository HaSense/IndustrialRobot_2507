#include <iostream>
using namespace std;

class Power
{
private:
    int kick;
    int punch;
public:
    Power(int kick=0, int punch=0):kick(kick), punch(punch) {}
    void show(){
        cout << "kick: " << kick << ", punch: " << punch << endl;
    }
    //연산자 오버로딩~!!!!!
    Power operator+ (Power other)
    {
        Power temp;
        temp.kick = this->kick + other.kick;
        temp.punch = this->punch + other.punch;
        return temp;
    }
    Power operator- (Power other)
    {
        Power temp;
        temp.kick = this->kick - other.kick;
        temp.punch = this->punch - other.punch;
        return temp;
    }
};

int main()
{
    Power a(3, 5);
    Power b(4, 6);
    Power c = a - b;

    a.show();
    b.show();
    c.show();

    return 0;
}
