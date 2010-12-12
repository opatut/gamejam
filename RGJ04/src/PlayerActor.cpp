#include "PlayerActor.hpp"

PlayerActor::PlayerActor(int id) {
	mId = id;
	mScore = 0;
}

void PlayerActor::InitializeAreas() {
	mAreas.clear();
	Area* a;

	a = new Area(Point(2, 2));
	a->SetColor(mColor);
	mAreas.push_back(a);

}

void PlayerActor::Update(float time_diff, sf::Vector2f offset) {
	UpdateAllAreas(time_diff, offset);
}
