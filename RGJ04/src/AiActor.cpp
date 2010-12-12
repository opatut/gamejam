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
	std::cout<<std::endl;
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
		Area& rndarea = mAreas[rand()%mAreas.size()];
		Point cl = rndarea.GetPointAt(rndarea.GetPoints().begin(), rand()%rndarea.GetPoints().size());
		std::cout << mName << " chooses to go to the center " << std::endl;
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
			rndarea = mAreas[rand()%mAreas.size()];
			cl = rndarea.GetPointAt(rndarea.GetPoints().begin(), rand()%rndarea.GetPoints().size());
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
				std::cout << mName << " chooses to increase its area " << std::endl;
				// INCREASE AREA
				// choose random point and move out
				Area& rndarea = mAreas[rand()%mAreas.size()];
				rndpoint = rndarea.GetPointAt(rndarea.GetPoints().begin(), rand()%rndarea.GetPoints().size());
			} else {
				std::cout << mName << " chooses to attack someone" << std::endl;
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
				std::cout << "Click = " << click.X << " " << click.Y << std::endl;
				++i;
			}
			if (Clicked(click))
				break;
		}
	}
	return true;
}

Point AiActor::ContactWithActor() {
	for(unsigned int i = 0; i < mAreas.size(); ++i) {
		Point p = mAreas[rand()%mAreas.size()].ContactWithActor(mId);
		if(p.X != -1 && p.Y != -1)
			return p;
	}
	return Point(-1,-1);
}
