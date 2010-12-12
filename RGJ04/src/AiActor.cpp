#include "AiActor.hpp"
#include "World.hpp"
#include <iostream>

AiActor::AiActor() {
	mMode = (rand()%2000 - 1000)/1000.f;
}

AiActor::AiActor(int id) {
	mId = id;
	mMode = (rand()%2000 - 1000)/1000.f;
	mScore = 0;
}


void AiActor::Update(float time_diff, sf::Vector2f offset) {
	mArea.Update(time_diff, offset);
}

bool AiActor::Evolve() {
	std::cout << "/*";
	Point click;
	// Think about a task to do
	// a) to the center (when there is someone or in offensive mode)
	// b) increase area (when area low or in defensive mode)
	// c) decrease other's area (when in contact)
	float task_a, task_b, task_c; // rate each from -1 to 1
	task_a = mMode;
	task_b = -mMode;
	task_c = -1;
	Point center = Point(FIELD_SIZE/BLOCKSIZE/2, FIELD_SIZE/BLOCKSIZE/2);
	int center_owner = World::get_mutable_instance().GetActorAtPoint(center);
	if (center_owner != mId && center_owner != -1) {
		task_a = (rand()%500)/1000.f + 0.5;
	}
	if (center_owner == mId) {
		//spread
		task_a = -1;
		task_b = (rand()%500)/1000.f + 0.5;
	}
	Point contact = ContactWithActor();
	if (!(contact == Point(-1,-1))) {
		task_c = (rand()%500)/1000.f + 0.5;
	}

	bool closer_to_center = false;
	if(task_a > task_b && task_a > task_c) {
		Point cl = mArea.GetPointAt(mArea.GetPoints().begin(), rand()%mArea.GetPoints().size());
		// TO THE CENTER
		// get some point close to center and area
		// get area's closest point
		// pick random adjacent point closer to center
		while(click == cl || !IsValidAddPoint(click, true) || cl.RealDistanceTo(center) < click.RealDistanceTo(center)) {
			int r = rand()%4;
			if(r == 0) click = cl+Point(-1, 0);
			if(r == 1) click = cl+Point( 1, 0);
			if(r == 2) click = cl+Point( 0,-1);
			if(r == 3) click = cl+Point( 0, 1);
			cl = mArea.GetPointAt(mArea.GetPoints().begin(), rand()%mArea.GetPoints().size());
		}
		if (Clicked(click))
			closer_to_center = true;
		else {
			std::cerr << "Epic fail" << std::endl;
			exit(1);
		}
	}
	if(closer_to_center == false) {
		while(true) {
			Point rndpoint;
			if (task_b > task_c) {
				// INCREASE AREA
				// choose random point and move out
				rndpoint = mArea.GetPointAt(mArea.GetPoints().begin(), rand()%mArea.GetPoints().size());
			} else {
				// DECREASE OTHER'S AREA
				rndpoint = ContactWithActor();
			}
			click = rndpoint;
			int r = rand();
			int i = 0;
			while(i <= 3 && (click == rndpoint || !IsValidAddPoint(click, true)) ) {
				if((r+i)%4 == 0) click = rndpoint+Point(-1, 0);
				if((r+i)%4 == 1) click = rndpoint+Point( 1, 0);
				if((r+i)%4 == 2) click = rndpoint+Point( 0,-1);
				if((r+i)%4 == 3) click = rndpoint+Point( 0, 1);
				++i;
			}
			if (Clicked(click))
				break;
		}
	}

	std::cout << "*/" << std::endl;
	return true;
}

Point AiActor::ContactWithActor() {
	return mArea.ContactWithActor(mId);
}
