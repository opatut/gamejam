#include "PlayerActor.hpp"

PlayerActor::PlayerActor(int id) {
	mId = id;
}

void PlayerActor::InitializeAreas() {
	Area* a;

	a = new Area(Point(mId*2, mId*2));
	a->SetColor(mColor);
	mAreas.push_back(a);

	a = new Area(Point(mId, mId));
	a->SetColor(mColor);
	mAreas.push_back(a);
}

void PlayerActor::Update(float time_diff) {
	UpdateAllAreas(time_diff);
}
