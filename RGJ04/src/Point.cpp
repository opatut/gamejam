#include "Point.hpp"
#include <cstdlib>
#include <math.h>

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

float Point::RealDistanceTo(const Point &p) {
	int dx = p.X - X;
	int dy = p.Y - Y;

	return sqrt(dx*dx+dy*dy);
}

bool Point::operator == (const Point &p2) {
	return (p2.X == X && p2.Y == Y);
}
