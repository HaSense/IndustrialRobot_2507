#include <iostream>
using namespace std;

class Car
{
private:
    int speed;

public:
    //Setter
    void setSpeed(int _speed){
        speed = _speed;
    }
    //Getter
    int getSpeed(){
        return speed;
    }

};
int main()
{
    Car myCar;
    myCar.setSpeed(100);
    cout << "현재 차의 스피드는 : " << myCar.getSpeed() << endl;

    return 0;
}
