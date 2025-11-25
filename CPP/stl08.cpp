#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<char> vec = {'A', 'B', 'C'};

    //vector<char>::iterator it;
    for(auto it=vec.begin(); it!=vec.end(); it++){
        cout << *it << "\t";
    }
    cout<<endl;

    return 0;
}
