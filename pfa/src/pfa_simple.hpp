#ifndef PFA_PFA_SIMPLE_HPP
#define PFA_PFA_SIMPLE_HPP

#include "list.hpp"
#include "path_finder.hpp"

namespace pfa {

enum class PointFlag {
    OPEN,
    CLOSE,
};

class SimplePathFinder: public PathFinder
{
    class Point;
    typedef List<Point*> PointList;
public:
    SimplePathFinder(World *world);
    const pfa::Point *findPath(int sx, int sy, int gx, int gy) override;

private:
    void pushOpen(Point *prev, const Dir *dir);
    void clear();
    void step();

    PointList open;
    Point **grid;

    Point *path;
    float minCost;
    Point *goal;
    bool repeat;
};

} // namespace pfa

#endif /* PFA_PFA_SIMPLE_HPP */