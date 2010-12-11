#include "Area.hpp"

Area::Area(Point starting_position) {
	mPoints.push_back(starting_position);
	mPoints.push_back(starting_position + Point(0,1));
	mPoints.push_back(starting_position + Point(1,1));
	mPoints.push_back(starting_position + Point(2,-1));
}

void Area::Update(float time_diff) {
	sf::Color inner_color(mColor.r, mColor.g, mColor.b, 200);

	mShape = sf::Shape();
	for(unsigned int i = 0; i < mPoints.size(); i++) {
		mShape.AddPoint(mPoints[i].X * BLOCKSIZE, mPoints[i].Y * BLOCKSIZE, inner_color, mColor);
	}
	mShape.SetOutlineWidth(1);
	mShape.EnableOutline(true);
}

void Area::Draw(sf::RenderTarget& target) {
	target.Draw(mShape);
}

void Area::SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color Area::GetColor() const {
	return mColor;
}
