#ifndef PFA_PATH_FINDER_HPP
#define PFA_PATH_FINDER_HPP

#include "direction.hpp"

namespace pfa {

class World {
  public:
	virtual float getTerrainCost(int x, int y) const = 0;
    virtual int getElevation(int x, int y) const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};

class Point
{
public:
    int x, y, z;
    Direction direction;

    Point(const World *world, Direction direction, int x, int y);
    Point(const Point *src);
    virtual ~Point();
    virtual const Point* prev() const;
    bool equal(const Point*);
};

class PathFinder
{
public:
    PathFinder(const World *world);
    virtual ~PathFinder();
    virtual const pfa::Point *findPath(int sx, int sy, int gx, int gy) = 0;

protected:
    const World *world;
};

} // namespace pfa

#endif /* PFA_PATH_FINDER_HPP */
