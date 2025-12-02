#include <iostream>
#include <cstdlib>
#include <memory>
#include <cmath>
using namespace std;

class MathUtils
{
public:
    static int abs(int a){
        if(a > 0)
            return a;
        else
            return -a;  
        //return a>0 ? a : -a;  //삼항연산자    
    }
    static int max(int a, int b){
        return a > b ? a : b;    
    }
    static int min(int a, int b){
        return a < b ? a : b;    
    }       
};
int main()
{
    MathUtils mu;
    mu.abs(-5);

    cout << MathUtils::abs(-5) << endl;
    cout << MathUtils::max(3, 7) << endl;
    cout << MathUtils::min(3, 7) << endl;   
    
    
    return 0;
}
