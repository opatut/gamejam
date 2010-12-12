#include "Actor.hpp"
#include <iostream>

Actor::Actor() {}

Actor::Actor(int id) {
	mId = id;
	mScore = 0;
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

bool Actor::Evolve() {
	return false;
}

bool Actor::IsValidAddPoint(const Point p, bool needs_outside) {
	if(p.X < 0 || p.Y < 0 || p.X > FIELD_SIZE/BLOCKSIZE || p.Y > FIELD_SIZE/BLOCKSIZE)
		return false;

	for(unsigned int i = 0; i < mAreas.size(); ++i) {

		int first_pos, last_pos;
		mAreas[i].GetFirstLastPos(p, first_pos, last_pos);


		if(mAreas[i].GetClosestPoint(p).DistanceTo(p) != 1 ||
		   (needs_outside && mAreas[i].IsPointInside(p)) ||
		   mAreas[i].PointOnPolygon(p) ||
		   last_pos == -1) // did not find 2 points to connect to
			return false;
	}
	return true;
}

Point Actor::GetClosestPoint(const Point p, int offset) {
	Point closest;
	int cl_area;
	int last_distance = 1000000;
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		Point cp = mAreas[i].GetClosestPoint(p);
		int d = cp.DistanceTo(p);
		if(d < last_distance) {
			closest = cp;
			cl_area = i;
			last_distance = d;
		}
	}
	if(offset == 0) return closest;
	else return mAreas[cl_area].GetPointAt(mAreas[cl_area].GetPoints().begin(),mAreas[cl_area].GetClosestPointNum(closest)+offset);
}

bool Actor::PointOnPolygon(const Point p) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		if(mAreas[i].PointOnPolygon(p))
			return true;
	}
	return false;
}
bool Actor::IsPointInside(const Point p) {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		if(mAreas[i].IsPointInside(p))
			return true;
	}
	return false;
}

bool Actor::Clicked(const Point p) {
	bool done = false;
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		if(mAreas[i].GetClosestPoint(p).DistanceTo(p) == 1) {
			done = mAreas[i].AddPoint(p);
		}
	}
	if(done)
		AddScore(5);
	return done;
}

void Actor::AddScore(int score) {
	mScore += score;
}

std::string Actor::ToString(bool current) {
	std::string score = boost::lexical_cast<std::string>(mScore);
	for(int i = score.length(); i <= 6; ++i) {
		score = " " + score;
	}
	if(current) return score + " * " + mName;
	else return score + "   " + mName;

}

void Actor::SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color Actor::GetColor() const {
	return mColor;
}

const int Actor::GetId() {
	return mId;
}


void Actor::SetName(const std::string& name) {
	mName = name;
}

const std::string& Actor::GetName() const {
	return mName;
}
