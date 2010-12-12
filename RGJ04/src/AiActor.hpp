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
	Point ContactWithActor();

private:
	float mMode; // -1 = passive/defensive, 0 = normal, 1 = active/offensive
};

#endif // AIACTOR_HPP
