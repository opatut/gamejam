#include "Point.hpp"
#include <cstdlib>

Point::Point() {
	X = 0;
	Y = 0;
}

Point::Point(int x, int y) {
	X = x;
	Y = y;
}

const Point Point::operator + (const Point &p2) {
	return Point(X + p2.X, Y + p2.Y);
}

int Point::DistanceTo(const Point &p) {
	return abs(p.X - X) + abs(p.Y - Y);
}
