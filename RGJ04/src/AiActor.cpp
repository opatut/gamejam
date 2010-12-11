#include "AiActor.hpp"
#include "World.hpp"
#include <iostream>

AiActor::AiActor() {
	mMode = (rand()%2000 - 1000)/1000.f;
}

AiActor::AiActor(int id) {
	mId = id;
	mMode = (rand()%2000 - 1000)/1000.f;
}

void AiActor::InitializeAreas() {
	mAreas.clear();
	Area* area;

	int a = 2;
	int b = FIELD_SIZE/BLOCKSIZE/2;
	int c = FIELD_SIZE/BLOCKSIZE-3;

	Point p(b,b);

	if(mId == 0) { p.X = a; p.Y = a; }
	if(mId == 1) { p.X = c; p.Y = c; }
	if(mId == 2) { p.X = c; p.Y = a; }
	if(mId == 3) { p.X = a; p.Y = c; }
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
	Point click;
	// Think about a task to do
	// a) to the center (when there is someone or in offensive mode)
	// b) increase area (when area low or in defensive mode)
	// c) decrease other's area (when in contact)
	float task_a, task_b, task_c, task_d; // rate each from -1 to 1
	task_a = mMode;
	task_b = -mMode;
	task_c = -1;
	Point center = Point(FIELD_SIZE/BLOCKSIZE/2, FIELD_SIZE/BLOCKSIZE/2);
	int center_owner = World::get_mutable_instance().GetActorAtPoint(center);
	if (center_owner != mId && center_owner != -1) {
		task_a = 1;
	}
	if (center_owner == mId) {
		//spread
		task_b = 1;
	}
	Point contact = ContactWithActor();
	if (!(contact == Point(-1,-1))) {
		task_c = 1;
	}

	if(task_a > task_b && task_a > task_c) {
		std::cout << mName << " chooses to go to the center " << std::endl;
		// TO THE CENTER
		// get some point close to center and area
		// get area's closest point
		Point cl = GetClosestPoint(center);
		// pick random adjacent point closer to center
		click = cl + Point(0,-1);
	} else if (task_b > task_c) {
		std::cout << mName << " chooses to increase its area " << std::endl;
		// INCREASE AREA
	} else {
		std::cout << mName << " chooses to attack someone" << center_owner<< std::endl;
		// DECREASE OTHER'S AREA
	}

	Clicked(click);
	return true;
}

Point AiActor::ContactWithActor() {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		Point p = mAreas[i].ContactWithActor(mId);
		if(p.X != -1 && p.Y != -1)
			return p;
	}
	return Point(-1,-1);
}
