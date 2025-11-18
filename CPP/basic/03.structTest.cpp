#include <iostream>

struct studentInfo {
    char bloodType;
    int stdNumber;
    float grade;
};
typedef struct studentInfo StudentInfo;

int main()
{
    using namespace std;

    StudentInfo si[2] = {
        {'O', 20031128, 3.5},
        { 'A', 19961219, 2.3 }
    };
    
    int cnt = sizeof(si) / sizeof(si[0]);
    for (int i = 0; i < cnt; i++) {
        cout << "혈액형 : " << si[i].bloodType << endl;
        cout << "학번 : " << si[i].stdNumber << endl;
        cout << "성적 : " << si[i].grade << endl << endl;
    }
    return 0;
}
