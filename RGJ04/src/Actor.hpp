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

	virtual void Update(float time_diff, sf::Vector2f offset) = 0;
	virtual void Draw(sf::RenderTarget& target);

	Point GetClosestPoint(const Point p);

	void Clicked(const Point p);

	void SetColor(const sf::Color color);
	const sf::Color GetColor() const;
protected:
	int mId;
	boost::ptr_vector<Area> mAreas;
	sf::Color mColor;

	void UpdateAllAreas(float time_diff, sf::Vector2f offset);
};

#endif // ACTOR_HPP
