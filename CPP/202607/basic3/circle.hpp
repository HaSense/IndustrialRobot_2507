#include <iostream> 

using namespace std; 
class Circle 
{
private:
	int radius;

public:
	Circle(){
		radius = 1;
		cout << "반지름 " << radius;
		cout << " 원 생성" << endl;
	}
	Circle(int r){
		radius = r;
		cout << "반지름 " << radius;
		cout << " 원 생성" << endl;
	}
	double getArea() {
		return 3.14*radius*radius;
	}	
}; 


