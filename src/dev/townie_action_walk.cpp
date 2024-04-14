#include "dev/townie_action_walk.hpp"

#include <stdio.h>
#include <math.h>

#include "linked_list.tcc"
#include "log.h"
#include "pfa_simple.hpp"
#include "game/world_data.hpp"

namespace dev {

TownieActionWalk::TownieActionWalk(Townie *townie, const pfa::Point *rpath)
	: TownieAction(townie)
{
	assert_not_null(rpath);
	const pfa::Point *curr = rpath;
	do {
		pfa::Point *point = new pfa::Point(curr);
		point->direction = curr->direction;
		path.push_back(point);
	} while((curr = curr->prev()) != nullptr);
	this->direction = (int) path.first()->direction;
}

TownieActionWalk::~TownieActionWalk()
{
	while(!path.isEmpty()) {
		pfa::Point *curr = path.remove_front();
		delete curr;
	}
}

void TownieActionWalk::onStartAction()
{
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
	if(update) {
		update = false;
		townie->setState(TownieStateDirection, this->direction);
		this->direction = (int) next->direction;
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

} // namespace dev
