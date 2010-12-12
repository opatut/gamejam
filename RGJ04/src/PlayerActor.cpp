#include "PlayerActor.hpp"

PlayerActor::PlayerActor(int id) {
	mId = id;
	mScore = 0;
}

void PlayerActor::Update(float time_diff, sf::Vector2f offset) {
	mArea.Update(time_diff, offset);
}
