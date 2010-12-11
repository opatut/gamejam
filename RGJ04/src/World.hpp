#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/cast.hpp>

#include "Definitions.hpp"
#include "Actor.hpp"
#include "PlayerActor.hpp"

class World {
public:
	World(sf::RenderWindow* app);
	void AddActor(Actor* actor, bool is_player = false);
	void Update(float time_diff);
	void Draw(sf::RenderTarget& target);
	bool Clicked(sf::Vector2i mouse_pos);
	sf::Vector2f GetOffset();

	PlayerActor* GetPlayerActor();
	void SetPlayerActor(PlayerActor* player_actor);

	Point PointAt(sf::Vector2f pos);

	Actor& GetActorById(int id);
private:
	boost::ptr_vector<Actor> mActors;
	sf::RenderWindow* mApp;
	sf::Shape mMouseHighlight;
	PlayerActor* mPlayerActor;
};

#endif // WORLD_HPP
