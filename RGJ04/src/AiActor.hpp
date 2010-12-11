#ifndef AIACTOR_HPP
#define AIACTOR_HPP

#include "Actor.hpp"
#include "Definitions.hpp"

class AiActor : public Actor {
public:
    AiActor();
	AiActor(int id);
	void InitializeAreas();

	void Update(float time_diff, sf::Vector2f offset);

	bool Evolve();
};

#endif // AIACTOR_HPP
