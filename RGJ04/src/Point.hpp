#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
	int X, Y;
    Point();
	Point(int x, int y);
	const Point operator + (const Point &p2);
	bool operator == (const Point &p2);
	int DistanceTo(const Point &p);
	float RealDistanceTo(const Point &p);
	bool IsInField() const;

};

#endif // POINT_HPP
