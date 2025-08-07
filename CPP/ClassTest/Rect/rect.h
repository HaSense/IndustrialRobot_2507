#pragma once

#include "point.h"
using namespace std;
class Rect
{
public:
	Point _topLeft;
	Point _bottomRight;

	void setTopLeft(const Point& topLeft)
	{
		_topLeft = topLeft;
	}
	Point getTopLeft() {
		return _topLeft;
	}
	void getTopLeft2() {
		cout << _topLeft.x << ", " << _topLeft.y << endl;
	}


};




