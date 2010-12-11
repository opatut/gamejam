#include "World.hpp"
#include <iostream>

World::World(sf::RenderWindow* app) {
	mApp = app;
}

void World::AddActor(Actor* actor, bool is_player) {
	mActors.push_back(actor);
	if(is_player) {
		mPlayerActor = dynamic_cast<PlayerActor*>(&(*(mActors.end() - 1)));
	}
}

void World::Update(float time_diff) {
	for(unsigned int i = 0; i < mActors.size(); ++i) {
		mActors[i].Update(time_diff, GetOffset());
	}

	sf::Vector2f mouse_position(mApp->GetInput().GetMouseX(), mApp->GetInput().GetMouseY());
	Point mouse_point = PointAt(mouse_position);

	if (mPlayerActor) {
		Point closest = mPlayerActor->GetClosestPoint(mouse_point);
		int d = closest.DistanceTo(mouse_point);
		if(d == 1) {
			mMouseHighlight = sf::Shape::Circle( mouse_point.X*BLOCKSIZE, mouse_point.Y*BLOCKSIZE, 3, sf::Color(255,255,255,100));
			mMouseHighlight.SetPosition(GetOffset());
		} else {
			mMouseHighlight = sf::Shape();
		}
	}
}


void World::Draw(sf::RenderTarget& target) {
	// draw grid
	sf::Shape line;
	for(int i = 0; i <= FIELD_SIZE; i+=BLOCKSIZE) {
		line = sf::Shape::Line(i,0,i,FIELD_SIZE,1,sf::Color(50,50,50));
		line.SetPosition(GetOffset());
		target.Draw(line);

		line = sf::Shape::Line(0,i,FIELD_SIZE,i,1,sf::Color(50,50,50));
		line.SetPosition(GetOffset());
		target.Draw(line);
	}
	for(int i = 0; i <= FIELD_SIZE; i+=BLOCKSIZE) {
		sf::Shape line = sf::Shape::Line(0,i,FIELD_SIZE,i,1,sf::Color(50,50,50));
		line.SetPosition(GetOffset());
		target.Draw(line);
	}

	for(unsigned int i = 0; i < mActors.size(); ++i) {
		mActors[i].Draw(target);
	}

	target.Draw(mMouseHighlight);
}


bool World::Clicked(sf::Vector2i mouse_pos) {
	Point p = PointAt(sf::Vector2f(mouse_pos.x, mouse_pos.y));
	mPlayerActor->Clicked(p);
	return true;
}

sf::Vector2f World::GetOffset() {
	return sf::Vector2f((WINDOW_WIDTH-FIELD_SIZE) / 2, (WINDOW_HEIGHT-FIELD_SIZE) / 2);
}

PlayerActor* World::GetPlayerActor() {
	return mPlayerActor;
}

void World::SetPlayerActor(PlayerActor* player_actor) {
	mPlayerActor = player_actor;
}

Point World::PointAt(sf::Vector2f pos) {
	return Point( round((pos.x - GetOffset().x) / BLOCKSIZE),
				  round((pos.y - GetOffset().y) / BLOCKSIZE));
}

Actor& World::GetActorById(int id) {
	for(unsigned int i = 0; i < mActors.size(); ++i) {
		if(mActors[i].GetId() == id)
			return mActors[i];
	}
}