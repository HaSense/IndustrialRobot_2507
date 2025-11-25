#include <iostream>
#include <string>
using namespace std;

int main()
{

    auto love = [](string a, string b){
        cout << a <<"보다 " << b <<"가 좋아" << endl;};

    love("돈", "너");
    love("냉면", "만두");
    
    return 0;
}
