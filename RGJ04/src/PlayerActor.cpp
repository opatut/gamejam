#include "PlayerActor.hpp"

PlayerActor::PlayerActor(int id) {
	mId = id;
	mScore = 0;
	mDisabled = false;
}

void PlayerActor::Update(float time_diff, sf::Vector2f offset) {
	if(mArea.GetPoints().size() < 3)
		mDisabled = true;
	mArea.Update(time_diff, offset);
}
