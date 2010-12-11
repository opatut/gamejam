#include "Actor.hpp"

Actor::Actor() {}

Actor::Actor(int id) {
	mId = id;
}

Actor::~Actor() {}

void Actor::InitializeAreas() {}

void Actor::UpdateAllAreas(float time_diff) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		mAreas[i].Update(time_diff);
	}
}

void Actor::Draw(sf::RenderTarget& target) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		mAreas[i].Draw(target);
	}
}

void Actor::SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color Actor::GetColor() const {
	return mColor;
}
