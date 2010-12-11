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

	void SetColor(const sf::Color color);
	const sf::Color GetColor() const;
private:
	sf::Shape mShape;
	sf::Color mColor;
	std::vector<Point> mPoints;
};

#endif // AREA_HPP
