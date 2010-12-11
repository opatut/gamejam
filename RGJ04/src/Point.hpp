#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
	int X, Y;
    Point();
	Point(int x, int y);
	const Point operator + (const Point &p2);

};

#endif // POINT_HPP
