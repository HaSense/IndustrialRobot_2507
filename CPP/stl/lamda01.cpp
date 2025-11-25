#include <iostream>

using namespace std;

int main()
{
    [](int x, int y){
        cout << "합은 " << x + y;
    }(2,3);
    cout<<endl;
    
    return 0;
}
