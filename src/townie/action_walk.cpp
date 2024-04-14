#include "townie/action_walk.hpp"

#include <math.h>

#include "game/enterable.hpp"
#include "linked_list.tcc"

#include "log.h"

TownieActionWalk::TownieActionWalk(pfa::PathFinder *_pathFinder, Townie *townie, int _dstX, int _dstY)
	: TownieAction(townie), pathFinder(_pathFinder), dstX(_dstX), dstY(_dstY), dst(nullptr)
{}

TownieActionWalk::TownieActionWalk(pfa::PathFinder *_pathFinder, Townie *townie, const Enterable *_dst)
	: TownieAction(townie), pathFinder(_pathFinder), dst(_dst)
{}

TownieActionWalk::~TownieActionWalk()
{
	while(!path.isEmpty()) {
		pfa::Point *curr = path.remove_front();
		delete curr;
	}
}

void TownieActionWalk::onStartAction()
{
    if(dst != nullptr) {
		const Entrance *first = dst->getEntrance()[0];
        dstX = first->getX();
        dstY = first->getY();
    }
	int srcX = (int) townie->getX();
	int srcY = (int) townie->getY();
	if(srcX == dstX && srcY == dstY) {
		return;
	}
    const pfa::Point *rpath = pathFinder->findPath(dstX, dstY, srcX, srcY);
	if(rpath == nullptr) {
		logp("Can't find path from (%d, %d) to (%d, %d)",srcX, srcY, dstX, dstY);
		return;
	}
	const pfa::Point *curr = rpath;
	do {
		pfa::Point *point = new pfa::Point(curr);
		point->direction = curr->direction;
		path.push_back(point);
	} while((curr = curr->prev()) != nullptr);
	this->direction = ((int) path.first()->direction + 4) % 8;
	townie->setState(TownieStateAction, TOWNIE_ACTION_WALKING);
}

void TownieActionWalk::onEndAction()
{
	townie->setState(TownieStateAction, TOWNIE_ACTION_STAND);
}

float TownieActionWalk::update(float dt)
{
	float timeUsed = 0;
	bool update = false;
start:
	pfa::Point *next = path.first();
	if(next == nullptr) {
		return 0;
	}
	if(update) {
		update = false;
		townie->setState(TownieStateDirection, this->direction);
		this->direction = ((int) next->direction + 4) % 8;
	}
	const float speed = 1.0f;
	float tx = townie->getX();
	float ty = townie->getY();
	float tz = townie->getZ();
	float dx = tx - next->x;
	float dy = ty - next->y;
	float dz = tz - next->z;
	float r = hypotf(dx, dy);
	float timeLeft = r / speed;
	if(dt > timeLeft) {
		path.remove_front();
		update = true;
		townie->setPosition(next->x, next->y, next->z);
		dt -= timeLeft;
		timeUsed += timeLeft;
		delete next;
		if(path.isEmpty()) return timeUsed;
		goto start;
	}
	float p = dt * speed / r;
	townie->setPosition(tx - dx * p, ty - dy * p, tz - dz * p);
	return dt;
}

bool TownieActionWalk::isFinished()
{
	return path.isEmpty();
}
