#include <iostream>
using namespace std;

char& find(char s[], int index){
    return s[index];
}

int main()
{
    char name[] = "Mike";
    cout << name << endl<<endl; //Mike

    //name[0] = 'S';
    find(name, 0) = 'S';
    cout << name << endl;

    char& ref = find(name, 2);
    ref = 't';
    //name[2] = 't';
    cout << name << endl; //Site

    return 0;
}
