#ifndef PLAYERACTOR_HPP
#define PLAYERACTOR_HPP

#include "Actor.hpp"

class PlayerActor : public Actor {
public:
	PlayerActor(int id);
	void Update(float time_diff, sf::Vector2f offset);
};

#endif // PLAYERACTOR_HPP
