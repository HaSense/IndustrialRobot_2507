#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

class Engine; //클래스 순서 선언. c++ 클래스 전방선언!!!

class Car
{
protected:
    Engine* engine = NULL; //레퍼런스로 가지고 있다.
public:
    Car()   //객체 초기화
    {

    }
    virtual ~Car()      //리소스 반환  
    {
        delete engine;  
    }
};

class Engine
{
public:
    virtual ~Engine()
    {
    }
    virtual string getBooster() 
    {
        return "부스터(없음)";
    }
};
class SuperEngine : public Engine
{
protected:
    string booster;
public:
    SuperEngine() 
    {
        booster = "부스터";
    }
    string getBooster() override
    {
        return "부스터(있음)";
    }
};
class SuperCar : public Car
{
public:
    SuperCar()
    {
        engine = new SuperEngine();
    }
    void Run()
    {
        cout << engine->getBooster() << "를 이용해서 달린다." << endl;
    }
};

int main()
{
    SuperCar myCar;
    myCar.Run();
    
    return 0;
}
