#include <iostream>
#include <cstdlib>
#include <memory>
using namespace std;

class TV 
{
protected:
    int size;   //스크린 크기 속성
public:
    TV() {  //기본 생성자  
        size = 20;
    }
    TV(int size){
        this->size = size;
    }
    //멤버메소드
    int getSize(){
        return this->size;
    }
};
class WideTV :public TV
{
protected:
    bool videoIn;
public:
    //defalt 생성자 없음
    WideTV(int size, bool videoIn):TV(size) {
        this->videoIn = videoIn;
    }
    //멤버 메소드
    bool getVideoIn() {
        return this->videoIn;
    }
};
class SmartTV : public WideTV 
{
protected:
    string ipAddress;
public:
    //default 생성자는 없음
    SmartTV(string ipAddr, int size) : WideTV(size, true) {
        this->ipAddress = ipAddr;
    }
    //멤버 함수
    string getIpAddr() {
        return ipAddress;
    }
};

int main()
{
    SmartTV htv("192.0.0.1", 32);
    cout << "size = " << htv.getSize() << endl;
    cout << "videoIn = " << htv.getVideoIn() << endl;
    cout << "IP = " << htv.getIpAddr() << endl; 

    return 0;
}
