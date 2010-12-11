#include "AiActor.hpp"
#include <iostream>

AiActor::AiActor() {}

AiActor::AiActor(int id) {
	mId = id;
}

void AiActor::InitializeAreas() {
	mAreas.clear();
	Area* area;

	int a = 2;
	int b = FIELD_SIZE/BLOCKSIZE/2;
	int c = FIELD_SIZE/BLOCKSIZE-3;

	Point p(b,b);

	if(mId == 0) { p.X = a; p.Y = a; }
	if(mId == 1) { p.X = a; p.Y = c; }
	if(mId == 2) { p.X = c; p.Y = a; }
	if(mId == 3) { p.X = c; p.Y = c; }
	if(mId == 4) { p.X = a; p.Y = b; }
	if(mId == 5) { p.X = c; p.Y = b; }
	if(mId == 6) { p.X = b; p.Y = a; }
	if(mId == 7) { p.X = b; p.Y = c; }


	area = new Area(p);
	area->SetColor(mColor);
	mAreas.push_back(area);
}


void AiActor::Update(float time_diff, sf::Vector2f offset) {
	UpdateAllAreas(time_diff, offset);
}

bool AiActor::Evolve() {
	std::cout << "Evolving AI actor " << mId << std::endl;
	return true;
}
