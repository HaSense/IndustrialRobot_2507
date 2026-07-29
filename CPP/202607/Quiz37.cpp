#include <iostream>

using namespace std;

class Calculator{
public:
    int plus(int a, int b){
        return a+b;
    }
    int minus(int a, int b){
        return a-b;
    }
    int multiple(int a, int b){
        return a*b;
    }
    double divide(int a, int b){
        return a / (double)b;
    }
};

int main(){

   int a, b;
   Calculator cal;
   cout << "첫번째 정수를 입력하세요 : ";
   cin >> a;
   cout << "두번째 정수를 입력하세요 : ";
   cin >> b;
   cout << "plus결과 : " << cal.plus(a, b)  << "입니다." << endl;
   cout << "minus결과 : " << cal.minus(a, b)  << "입니다." << endl;
   cout << "multiple결과 : " << cal.multiple(a, b)  << "입니다." << endl;
   cout << "divide결과 : " << cal.divide(a, b)  << "입니다." << endl;
}
