#include "Area.hpp"
#include <iostream>

Area::Area(Point starting_position) {
	mPoints.push_back(starting_position);
	mPoints.push_back(starting_position + Point(0,1));
	mPoints.push_back(starting_position + Point(1,1));
	mPoints.push_back(starting_position + Point(1,0));
}

void Area::Update(float time_diff, sf::Vector2f offset) {
	sf::Color inner_color(mColor.r, mColor.g, mColor.b, 200);

	mShape = sf::Shape();
	for(unsigned int i = 0; i < mPoints.size(); i++) {
		mShape.AddPoint(mPoints[i].X * BLOCKSIZE, mPoints[i].Y * BLOCKSIZE, inner_color, mColor);
	}
	mShape.SetOutlineWidth(1);
	mShape.EnableOutline(true);
	mShape.SetPosition(offset);
}

void Area::Draw(sf::RenderTarget& target) {
	target.Draw(mShape);
}

Point Area::GetClosestPoint(const Point p) {
	Point closest = p;
	int last_distance = 1000000;

	for(unsigned int i = 0; i < mPoints.size(); i++) {
		int d = mPoints[i].DistanceTo(p);
		if(d < last_distance) {
			closest = mPoints[i];
			last_distance = d;
		}
	}
	return closest;
}

void Area::SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color Area::GetColor() const {
	return mColor;
}
