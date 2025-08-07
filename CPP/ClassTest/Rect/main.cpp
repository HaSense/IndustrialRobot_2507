#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include "rect.h"

using namespace std;

int main()
{
    Rect rc1;
    Point pt1;
    pt1.x = 10;
    pt1.y = 20;

    rc1.setTopLeft(pt1);
    Point rPt = rc1.getTopLeft();
    cout << "X : " << rPt.x << endl;
    cout << "Y : " << rPt.y << endl << endl;

    rc1.getTopLeft2();

    return 0;
}
