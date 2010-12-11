#include "Actor.hpp"
#include <iostream>

Actor::Actor() {}

Actor::Actor(int id) {
	mId = id;
}

Actor::~Actor() {}

void Actor::InitializeAreas() {
	mAreas.clear();
}

void Actor::UpdateAllAreas(float time_diff, sf::Vector2f offset) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		mAreas[i].Update(time_diff, offset);
	}
}

void Actor::Draw(sf::RenderTarget& target) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		mAreas[i].Draw(target);
	}
}

Point Actor::GetClosestPoint(const Point p) {
	Point closest;
	int last_distance = 1000000;
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		Point cp = mAreas[i].GetClosestPoint(p);
		int d = cp.DistanceTo(p);
		if(d < last_distance) {
			closest = cp;
			last_distance = d;
		}
	}
	return closest;
}

void Actor::Clicked(const Point p) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		if(mAreas[i].GetClosestPoint(p).DistanceTo(p) == 1) {
			mAreas[i].AddPoint(p);
		}
	}

}

void Actor::SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color Actor::GetColor() const {
	return mColor;
}
