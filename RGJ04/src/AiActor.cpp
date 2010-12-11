#include "AiActor.hpp"
#include <iostream>

AiActor::AiActor() {}

AiActor::AiActor(int id) {
	mId = id;
}

void AiActor::InitializeAreas() {
	mAreas.clear();
	Area* a;

	Point p(2,2);

	if(mId == 1 || mId == 3) p.X = FIELD_SIZE/BLOCKSIZE - 3;
	if(mId == 1 || mId == 2) p.Y = FIELD_SIZE/BLOCKSIZE - 3;

	a = new Area(p);
	a->SetColor(mColor);
	mAreas.push_back(a);
}


void AiActor::Update(float time_diff, sf::Vector2f offset) {
	UpdateAllAreas(time_diff, offset);
}

bool AiActor::Evolve() {
	std::cout << "Evolving AI actor " << mId << std::endl;
	return true;
}
