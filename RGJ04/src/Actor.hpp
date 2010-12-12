#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/lexical_cast.hpp>
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

	virtual bool Evolve();

	bool IsValidAddPoint(const Point p, bool needs_outside = false);

	Point GetClosestPoint(const Point p, int offset = 0);
	bool PointOnPolygon(const Point p);
	bool IsPointInside(const Point p);

	bool Clicked(const Point p);
	void AddScore(int score);

	std::string ToString(bool current);

	void SetColor(const sf::Color color);
	const sf::Color GetColor() const;

	const int GetId();

	void SetName(const std::string& name);
	const std::string& GetName() const;
protected:
	int mId;
	std::string mName;
	int mScore;
	boost::ptr_vector<Area> mAreas;
	sf::Color mColor;

	void UpdateAllAreas(float time_diff, sf::Vector2f offset);
};

#endif // ACTOR_HPP
