#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/cast.hpp>

#include "Definitions.hpp"
#include "Actor.hpp"
#include "PlayerActor.hpp"

class World : public boost::serialization::singleton<World> {
public:
	World();

	void Reset();

	void SetWindow(sf::RenderWindow* app);

	void AddActor(Actor* actor, bool is_player = false);
	void Update(float time_diff);
	void Draw(sf::RenderTarget& target);
	bool Clicked(sf::Vector2i mouse_pos);
	bool PointAdded(const Point p, int actor_id);
	bool InsideOtherArea(const Point p, int own_id);
	sf::Vector2f GetOffset();

	PlayerActor* GetPlayerActor();
	void SetPlayerActor(PlayerActor* player_actor);

	Point PointAt(sf::Vector2f pos);

	Actor& GetActorById(int id);
	int GetActorAtPoint(const Point p);
private:
	boost::ptr_vector<Actor> mActors;
	sf::RenderWindow* mApp;
	sf::Shape mMouseHighlight;
	PlayerActor* mPlayerActor;
};

#endif // WORLD_HPP
