#include "pfa_simple.hpp"

#include <math.h>

#include "log.h"
#include "list.tcc"

namespace pfa {

#define PRIORITY_CONSTANT 1.5f

class SimplePathFinder::Point: public pfa::Point
{
public:
    Point(int x, int y, Direction direction, const World *world)
        : pfa::Point(world, direction, x, y)
    {
        flag = PointFlag::OPEN;
        this->_prev = nullptr;
        this->pointCost = world->getTerrainCost(x, y);
    }

    const pfa::Point *prev() const override
    {
        return _prev;
    }

    void setPriorityScore(float totalCost, Point *goal)
    {
        this->totalCost = totalCost;
        this->priorityScore = totalCost + PRIORITY_CONSTANT * hypotf(this->x - goal->x, this->y - goal->y);
    }

    void dump()
    {
        logp("Point@%p(x=%d, y=%d, z=%d, dir=%d, prev=@%p)", this, x, y, z, direction, _prev);
    }

    float pointCost;
    float totalCost;
    float priorityScore;
    PointFlag flag;
    Point *_prev;
};

SimplePathFinder::SimplePathFinder(World *world)
    : PathFinder(world)
{
	this->grid = new Point*[world->getWidth() * world->getHeight()];
	path = NULL;
	minCost = 0.0f;

    int count = world->getWidth() * world->getHeight();
    for(int i=0; i<count; i++) {
    	this->grid[i] = nullptr;
    }
}

void SimplePathFinder::clear() {
	this->open.clear();
	path = NULL;
	minCost = INFINITY;
    goal = NULL;
    int count = world->getWidth() * world->getHeight();
    for(int i=0; i<count; i++) {
    	Point *p = this->grid[i];
    	if(p != NULL) {
    		delete p;
    		this->grid[i] = NULL;
    	}
    }
}

const pfa::Point *SimplePathFinder::findPath(int sx, int sy, int gx, int gy)
{
    if(sx==gx && sy == gy){
        return NULL;
    }
    clear();
    this->goal = new Point(gx, gy, Direction::UNKNOWN, this->world);
    if(goal->pointCost == INFINITY) {
    	return nullptr;
    }
    this->open.push(new Point(sx, sy, Direction::UNKNOWN, this->world));
    if(open[0]->pointCost == INFINITY) {
    	delete open[0];
    	return nullptr;
    }
    int limit = (int) fmaxf(5000, 30 * hypotf(sx - gx, sy - gy));
    int count = 0;
    while(this->open.size()>0 && this->path==NULL && limit>=0) {
        this->step();
        limit --;
        count++;
    }
    delete goal;
    if(this->path == NULL){
        return NULL;
    }
    return this->path;
}

void SimplePathFinder::pushOpen(Point *prev, const Dir *dir){
    Point *p = new Point(prev->x + dir->dx, prev->y + dir->dy, dir->id, this->world);
    if(p->pointCost == INFINITY) {
    	delete p;
        return;
    }
    const float totalCost = prev->totalCost + (p->pointCost + prev->pointCost) * dir->dist / 2;

    p->setPriorityScore(totalCost, this->goal);
    p->_prev = prev;
    Point *o = this->grid[this->world->getWidth() * p->y + p->x];
    if(o != NULL){
        if(o->flag == PointFlag::OPEN){
            if(p->totalCost < o->totalCost){
                goto fix_value;
            }
        } else {
            if(this->repeat && p->totalCost < o->totalCost){
                this->open.push(o);
                goto fix_value;
            }
        }
        delete p;
        return;

    fix_value:
        o->totalCost = p->totalCost;
        o->_prev = p->_prev;
        o->direction = Dir::getDir(o->x, o->y, o->_prev->x, o->_prev->y)->id;
        delete p;
        return;
    }
    if(p->equal(this->goal)){
        if(this->path != nullptr){
            if(this->path->totalCost < p->totalCost){
            	delete p;
                return;
            }
        }
        this->path = p;
        this->minCost = p->totalCost;
        return;
    }
    this->open.push(p);
    this->grid[this->world->getWidth() * p->y + p->x] = p;
}

void SimplePathFinder::step() {
    Point *p = NULL;

    float minScore = INFINITY;
    int minIndex = -1;
    for(int i=0; i<open.size(); i++){
        Point *point = open[i];
        float score = point->priorityScore;
        if(score < minScore) {
            p = point;
            minScore = score;
            minIndex = i;
        }
    }
    if(minIndex == -1) {
        for(int i=0; i<open.size(); i++){
            delete open[i];
        }
        open.clear();
        return;
    }
    this->open.remove(minIndex);

    p->flag = PointFlag::CLOSE;
    switch(p->direction){
    case Direction::DOWN:
        this->pushOpen(p, Dir::down);
        this->pushOpen(p, Dir::down_left);
        this->pushOpen(p, Dir::down_right);
        this->pushOpen(p, Dir::right);
        this->pushOpen(p, Dir::left);
        break;
    case Direction::UP:
        this->pushOpen(p, Dir::up);
        this->pushOpen(p, Dir::up_left);
        this->pushOpen(p, Dir::up_right);
        this->pushOpen(p, Dir::right);
        this->pushOpen(p, Dir::left);
        break;
    case Direction::RIGHT:
        this->pushOpen(p, Dir::right);
        this->pushOpen(p, Dir::up_right);
        this->pushOpen(p, Dir::down_right);
        this->pushOpen(p, Dir::down);
        this->pushOpen(p, Dir::up);
        break;
    case Direction::LEFT:
        this->pushOpen(p, Dir::left);
        this->pushOpen(p, Dir::down_left);
        this->pushOpen(p, Dir::up_left);
        this->pushOpen(p, Dir::down);
        this->pushOpen(p, Dir::up);
        break;
    case Direction::DOWN_LEFT:
        this->pushOpen(p, Dir::down_left);                       
        this->pushOpen(p, Dir::down);
        this->pushOpen(p, Dir::left);
        this->pushOpen(p, Dir::up_left);
        this->pushOpen(p, Dir::down_right);
        break;
    case Direction::UP_LEFT:
        this->pushOpen(p, Dir::up_left);
        this->pushOpen(p, Dir::left);
        this->pushOpen(p, Dir::up);
        this->pushOpen(p, Dir::down_left);
        this->pushOpen(p, Dir::up_right);
        break;
    case Direction::DOWN_RIGHT:
        this->pushOpen(p, Dir::down_right);
        this->pushOpen(p, Dir::right);
        this->pushOpen(p, Dir::down);
        this->pushOpen(p, Dir::down_left);
        this->pushOpen(p, Dir::up_right);
        break;
    case Direction::UP_RIGHT:
        this->pushOpen(p, Dir::up_right);
        this->pushOpen(p, Dir::up);
        this->pushOpen(p, Dir::right);
        this->pushOpen(p, Dir::up_left);
        this->pushOpen(p, Dir::down_right);
        break;
    default:
        this->pushOpen(p, Dir::down);
        this->pushOpen(p, Dir::up);
        this->pushOpen(p, Dir::right);
        this->pushOpen(p, Dir::left);
        this->pushOpen(p, Dir::down_left);
        this->pushOpen(p, Dir::up_left);
        this->pushOpen(p, Dir::down_right);
        this->pushOpen(p, Dir::up_right);
    }
}

} // namespace pfa