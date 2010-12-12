#include "Area.hpp"
#include "World.hpp"
#include <iostream>

Area::Area(Point starting_position) {
	mPoints.push_back(starting_position);
	mPoints.push_back(starting_position + Point(0,1));
	mPoints.push_back(starting_position + Point(1,1));
	mPoints.push_back(starting_position + Point(1,0));
}

void Area::Update(float time_diff, sf::Vector2f offset) {
	/*sf::Color inner_color(mColor.r, mColor.g, mColor.b, 200);

	mShape = sf::Shape();
	for(unsigned int i = 0; i < mPoints.size(); i++) {
		mShape.AddPoint(mPoints[i].X * BLOCKSIZE, mPoints[i].Y * BLOCKSIZE, inner_color, mColor);
	}
	mShape.SetOutlineWidth(1);
	mShape.EnableOutline(true);
	mShape.EnableFill(true);
	mShape.SetPosition(offset);*/
	mOffset = offset;
}

void Area::Draw(sf::RenderTarget& target) {
	//target.Draw(mShape);
	for(unsigned int i = 0; i < mPoints.size(); i++) {
		Point after;
		if(i == mPoints.size()-1) after = mPoints.front();
		else after = mPoints[i+1];

		sf::Shape line = sf::Shape::Line(mPoints[i].X * BLOCKSIZE,
										 mPoints[i].Y * BLOCKSIZE,
										 after.X * BLOCKSIZE,
										 after.Y * BLOCKSIZE,
										 3.f,
										 mColor);
		line.SetPosition(mOffset);
		target.Draw(line);

		sf::Shape circle = sf::Shape::Rectangle(mPoints[i].X*BLOCKSIZE - 4, mPoints[i].Y*BLOCKSIZE - 3, 7, 7, mColor);
		circle.SetPosition(mOffset);
		target.Draw(circle);
	}
}

Point Area::GetClosestPoint(const Point p) {
	Point closest = p;
	int last_distance = 1000000;

	for(unsigned int i = 0; i < mPoints.size(); i++) {
		int d = mPoints[i].DistanceTo(p);
		if(d < last_distance) {
			closest = mPoints[i];
			last_distance = d;
		}
	}
	return closest;
}

bool Area::PointOnPolygon(const Point p) {
	return GetClosestPoint(p).DistanceTo(p) == 0;
}

void Area::SetColor(const sf::Color color) {
	mColor = color;
}

const sf::Color Area::GetColor() const {
	return mColor;
}

bool Area::AddPoint(Point p) {
	int first_pos = -1;
	int last_pos = -1;

	for(int i = 0; i < mPoints.size(); ++i) {
		if(mPoints[i].DistanceTo(p) == 1) {
			if(first_pos == -1) {
				first_pos = i;
			} else {
				last_pos = i;
			}
		}
	}

	if (last_pos == -1) {
		first_pos = -1;
		for(int i = 0; i < mPoints.size(); ++i) {
			if(mPoints[i].RealDistanceTo(p) < 2) {
				if(first_pos == -1) {
					first_pos = i;
				} else {
					last_pos = i;
				}
			}
		}
	}

	if (last_pos == -1)
		false;

	// check if first and last are swapped (first_pos is always < last_pos)
	if(last_pos - first_pos > mPoints.size()-last_pos+first_pos ) {
		std::swap(last_pos, first_pos);
	}

	//std::cout << "erasing between: " << first_pos << " and " << last_pos << std::endl;

	// remove everything in between
	int i = first_pos + 1;
	while(i < last_pos) {
		mPoints.erase(mPoints.begin()+i);
		std::cout << "> erase " << i << std::endl;
		last_pos--;
	}

	// insert new point after first_pos
	mPoints.insert(mPoints.begin()+first_pos+1, p);
	return true;
}


Point Area::GetPointAt(std::vector<Point>::iterator i, int offset) {
	while(offset > 0) {
		i++;
		if(i == mPoints.end()) i = mPoints.begin();
		--offset;
	}

	while(offset < 0) {
		if(i == mPoints.begin()) i = mPoints.end();
		i--;
		++offset;
	}

	return *i;
}

bool Area::IsPointInside(const Point p) {
	// if point is part of polygon, return false
	if(PointOnPolygon(p))
		return false;

	Point up = p;

	// go up from point, count crossings
	int num_crossings = 0;
	int last_was_middle = 0;

	while(up.Y >= 0) {
		if(PointOnPolygon(up)) {
			std::vector<Point>::iterator up_iter = std::find(mPoints.begin(), mPoints.end(), up);

			Point before = GetPointAt(up_iter,-1);
			Point after = GetPointAt(up_iter,+1);

			int dx = abs(before.X - after.X);
			if(last_was_middle == 0) {
				if(dx == 2) {
					// they are on different sides
					num_crossings++;
				} else if(dx == 0 && before.X != up.X) {
					// they are on the same side
				} else {
					// one of them (or two) are in the middle
					last_was_middle = before.X - up.X;
				}
			} else {
				// last_was_middle = -1  ==>  before was on left side
				if (dx == 0 && before.X == up.X) {
					// both in middle, continue
				} else if (after.X - up.X == last_was_middle) { // after is now on the same side of up as before was
					// no crossing
					last_was_middle = 0;
				} else {
					num_crossings++;
					last_was_middle = 0;
				}
			}
		}
		up.Y -= 1;
	}
	return num_crossings%2==1;
}

Point Area::ContactWithActor(int own_id) {
	for(unsigned int i = 0; i < mPoints.size(); ++i) {
		for(int x = -1; x <= 1; ++x) {
			for(int y = -1; y <= 1; ++y) {
				if(y!=0 || x!=0) {
					int owner = World::get_mutable_instance().GetActorAtPoint(Point(x+mPoints[i].X,y+mPoints[i].Y));
					if(owner != own_id && owner != -1) {
						return mPoints[i];
					}
				}
			}
		}
	}
	return Point(-1,-1);
}
