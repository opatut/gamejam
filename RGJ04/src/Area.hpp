#ifndef AREA_HPP
#define AREA_HPP

#include <SFML/Graphics.hpp>

#include "Definitions.hpp"
#include "Point.hpp"

class Area {
public:
	Area(Point starting_position);
	void Update(float time_diff, sf::Vector2f offset);
	void Draw(sf::RenderTarget& target);

	Point GetClosestPoint(const Point p);
	bool PointOnPolygon(const Point p);

	void SetColor(const sf::Color color);
	const sf::Color GetColor() const;

	bool AddPoint(Point p);
	std::vector<Point> GetPoints();
	Point GetPointAt(std::vector<Point>::iterator i, int offset = 0);
	bool IsPointInside(const Point p);
	Point ContactWithActor(int own_id);
private:
	sf::Vector2f mOffset;
	//sf::Shape mShape;
	sf::Color mColor;
	std::vector<Point> mPoints;
};

#endif // AREA_HPP
