#include "Area.hpp"

Area::Area(Point starting_position) {
	mPoints.push_back(starting_position);
	mPoints.push_back(starting_position + Point(0,1));
	mPoints.push_back(starting_position + Point(1,1));
	mPoints.push_back(starting_position + Point(1,0));
}

void Update(float time_diff) {
	mShape = sf::Shape();
}

void Area::Draw(sf::RenderTarget& target) {
	target.Draw(mShape);
}

void SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color GetColor() const {
	return mColor;
}
