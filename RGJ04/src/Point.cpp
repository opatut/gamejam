#include "Point.hpp"

Point::Point() {
	X = 0;
	Y = 0;
}

Point::Point(int x, int y) {
	X = x;
	Y = y;
}

const Point operator + (const Point &p2) {
	return Point(X + p2.X, Y + p2.Y);
}
