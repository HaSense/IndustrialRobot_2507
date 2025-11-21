#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    int* p;

    p = new int[5];

    for(int i=0; i<5; i++) {
        p[i] = i + 1;       // 1 2 3 4 5
        cout << p[i] << " "; 
    }
    cout<<endl;

    delete [] p;

    return 0;
}
