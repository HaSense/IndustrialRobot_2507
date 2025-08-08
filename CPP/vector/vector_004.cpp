#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
class Score
{
public:
    int kor;
    int eng;
    int math;
};

int main()
{
    Score s1, s2;
    s1.kor = 100;
    s1.eng = 100;
    s1.math = 90;
    s2.kor = 100;
    s2.eng = 90;
    s2.math = 95;

    vector<Score> myClassScore;
    myClassScore.push_back(s1);
    myClassScore.push_back(s2);

    for (Score s : myClassScore) {
        cout << "국어성적 : " << s.kor << endl;
        cout << "영어성적 : " << s.eng << endl;
        cout << "수학성적 : " << s.math << endl;
        cout << endl;
    }

    return 0;
}


