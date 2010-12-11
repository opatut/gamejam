#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <boost/ptr_container/ptr_vector.hpp>
#include <SFML/Graphics.hpp>
#include "Area.hpp"

class Actor {
public:
	Actor();
	Actor(int id);
	virtual ~Actor() = 0;

	virtual void InitializeAreas();

	virtual void Update(float time_diff) = 0;
	virtual void Draw(sf::RenderTarget& target);

	void SetColor(const sf::Color color);
	const sf::Color GetColor() const;
protected:
	int mId;
	boost::ptr_vector<Area> mAreas;
	sf::Color mColor;

	void UpdateAllAreas(float time_diff);
};

#endif // ACTOR_HPP
