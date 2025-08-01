#include <iostream>

using namespace std;

class Car
{
public: 
    int speed;
    string brand;
    string color;
    //생성자
    Car() :speed(0), brand("현대"), color("하얀색")
    {
        cout << "Car 객체가 생성되었습니다." << endl;
    }
    //인자가 3개 있는 생성자
    Car(int speed, string brand, string color) 
        : speed(160), brand("Kia"), color("검은색"){
       
        //this->speed = speed;
        //this->brand = brand;
        //this->color = color;
    }

    ~Car()
    {
        cout << "Car 객체가 소멸되었습니다." << endl;
    }
};
int main()
{
    Car myCar;  //객체를 Stack 메모리에 만들었다.
    cout << "speed : " << myCar.speed << endl;
    cout << "brand : " << myCar.brand << endl;
    cout << "color : " << myCar.color << endl << endl;

    Car* myCar2 = new Car();    //객체를 Heap 메모리에 만들었다.
    cout << "speed : " << myCar2->speed << endl;
    cout << "brand : " << myCar2->brand << endl;
    cout << "color : " << myCar2->color << endl << endl;

    Car* myCar3 = new Car(160, "Kia", "검은색");
    cout << "speed : " << myCar3->speed << endl;
    cout << "brand : " << myCar3->brand << endl;
    cout << "color : " << myCar3->color << endl;

    delete myCar2;
    delete myCar3;
}




