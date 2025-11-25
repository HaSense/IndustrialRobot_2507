#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(int n) {
    cout << n <<" ";
}

int main()
{
    vector<int> vec = {1,2,3,4,5};

    for_each(vec.begin(), vec.end(), print);
    cout<<endl;
    for_each(vec.begin(), vec.end(), [](int n){cout << n <<" ";});
    cout<<endl;
    
    return 0;
}
