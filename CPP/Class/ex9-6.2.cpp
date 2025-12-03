#include <iostream>
#include <memory>
using namespace std;

class Calculator 
{
public:
	//순수 가상함수 3개를 만드세요.
    virtual int add(int a, int b)=0; //순수가상함수, 구현하지 않았음
	virtual int subtract(int a, int b)=0; 	
	virtual double average(int a [], int size)=0;
};
class GoodCalc : public Calculator 
{
public:
	int add(int a, int b) override { 
        return a + b; 
    }	
	int subtract(int a, int b) override{ 
        return a - b; 
    }	
	double average(int a [], int size) override{
		double sum = 0;
		for(int i=0; i<size; i++) 
			sum += a[i];
		return sum/size;
	}	
};

int main() 
{
	int a[] = {1,2,3,4,5};
	
	auto p = make_unique<GoodCalc>();

    cout << p->add(2, 3) << endl;
	cout << p->subtract(2, 3) << endl;
	cout << p->average(a, 5) << endl;
    
    return 0;
}
