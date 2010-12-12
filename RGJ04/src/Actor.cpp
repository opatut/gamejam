#include "Actor.hpp"
#include "World.hpp"
#include <iostream>

Actor::Actor() {}

Actor::Actor(int id) {
	mId = id;
	mScore = 0;
	mDisabled = false;
}

Actor::~Actor() {}

void Actor::InitializeArea() {
	int a = 4;
	int b = FIELD_SIZE/BLOCKSIZE/2;
	int c = FIELD_SIZE/BLOCKSIZE-a;

	Point p(b,b);

	if(mId == 0) { p.X = a; p.Y = a; }
	if(mId == 1) { p.X = c; p.Y = c; }
	if(mId == 2) { p.X = c; p.Y = a; }
	if(mId == 3) { p.X = a; p.Y = c; }
	if(mId == 4) { p.X = a; p.Y = b; }
	if(mId == 5) { p.X = c; p.Y = b; }
	if(mId == 6) { p.X = b; p.Y = a; }
	if(mId == 7) { p.X = b; p.Y = c; }

	mArea = Area(p);
	mArea.SetColor(mColor);
}

void Actor::Draw(sf::RenderTarget& target) {
	if(mDisabled)
		return;
	mArea.Draw(target);
}

bool Actor::Evolve() {
	return false;
}

bool Actor::IsValidAddPoint(const Point p, bool needs_outside) {
	if(!p.IsInField())
		return false;


	int first_pos, last_pos;
	mArea.GetFirstLastPos(p, first_pos, last_pos);


	return !(mArea.GetClosestPoint(p).DistanceTo(p) != 1 ||
		   (needs_outside && mArea.IsPointInside(p)) ||
		   mArea.PointOnPolygon(p) ||
		   last_pos == -1 || // did not find 2 points to connect to
		   World::get_mutable_instance().InsideOtherArea(p, mId) );
}

Point Actor::GetClosestPoint(const Point p, int offset) {
	return mArea.GetClosestPoint(p, offset);;
}

bool Actor::PointOnPolygon(const Point p) {
	return mArea.PointOnPolygon(p);
}

bool Actor::IsPointInside(const Point p) {
	return mArea.IsPointInside(p);
}

void Actor::RemovePoint(const Point p) {
	mArea.RemovePoint(p);
}

int Actor::RemovePointsInsideArea(Area& area) {
	int n = 0;
	for(unsigned int i = 0; i < mArea.GetPoints().size(); ++i) {
		Point p = mArea.GetPointAt(mArea.GetPoints().begin(), i);
		if(area.IsPointInside(p)) {
			RemovePoint(p);
			n++;
		}
	}
	return n;
}

bool Actor::Clicked(const Point p) {
	if(IsValidAddPoint(p)) {
		if (mArea.AddPoint(p)) {
			AddScore(1);
			AddScore(10*World::get_mutable_instance().PointAdded(p, mId));
			return true;
		}
	}
	return false;
}

int Actor::GetScore() {
	return mScore;
}

void Actor::AddScore(int score) {
	mScore += score;
}

std::string Actor::ToString(bool current) {
	std::string score = boost::lexical_cast<std::string>(mScore);
	for(int i = score.length(); i <= 6; ++i) {
		score = " " + score;
	}
	if(mDisabled) return score + " X " + mName;
	if(current) return score + " * " + mName;
	return score + "   " + mName;

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

Area& Actor::GetArea(){
	return mArea;
}

bool Actor::IsDisabled() {
	return mDisabled;
}
