#include "path_finder.hpp"

namespace pfa {

Point::Point(const World *world, Direction direction, int x, int y) {
	this->x = x;
	this->y = y;
    this->z = world->getElevation(x, y);
	this->direction = direction;
}

Point::Point(const Point *src)
{
	this->x = src->x;
	this->y = src->y;
    this->z = src->z;
	this->direction = src->direction;
}

Point::~Point()
{}

const Point* Point::prev() const
{
    return nullptr;
}
bool Point::equal(const Point *o) {
	return this->x == o->x && this->y == o->y;
}

PathFinder::PathFinder(const World *_world)
    : world(_world) {}

PathFinder::~PathFinder()
{}

} // namespace pfa


