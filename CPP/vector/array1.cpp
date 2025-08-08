#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;
int main()
{
    int arr1[5];
    int arr2[5] = { 1, 2, 3, 4, 5 };
    int arr3[5] = { 0, };

    //i,j,k,l,m,n...
    int n = 3;
    for (int i = 0; i < 5; i++)
    {
        arr1[i] = n++;
    }
    cout << endl;
    return 0;
}
