#include <iostream>

using namespace std;

class Horse
{ };

class IWing
{
public:
    void fly()
    {
        cout << "날아 다닌다~!!!" << endl;
    }
};
class IWing2
{
public:
    void fly()
    {
        cout << "잘 날아 다닌다~!!!" << endl;
    }
};

class Unicorn : public Horse, public IWing2
{

};

int main()
{
    Unicorn uni;
    uni.fly();
    return 0;
}
