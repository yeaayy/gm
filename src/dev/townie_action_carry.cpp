#include "dev/townie_action_carry.hpp"

#include "dev/townie_action_wait.hpp"
#include "dev/townie_action_walk.hpp"

namespace dev {

TownieActionCarry::TownieActionCarry(Townie *townie, pfa::PathFinder *pathFinder, Building *src, Building *dst)
	: TownieAction(townie)
{
	this->pathFinder = pathFinder;
	this->src = src;
	this->dst = dst;
	this->stage = 0;
}

float TownieActionCarry::update(float dt)
{
	if(stage == 0) {
		townie->addActionFront(new dev::TownieActionWait(townie, 1, false));
		auto entrance = src->getEntrance()[0];
		townie->addActionFront(new dev::TownieActionWalk(townie, pathFinder->findPath(entrance->getX(), entrance->getY(), (int) townie->getX(), (int) townie->getY())));
	} else if(stage == 1) {
		if(!src->getStorage()->transferToVia(townie->getCarried(), dst->getStorage())) {
			stage = 4;
			return 0;
		}
		townie->addActionFront(new dev::TownieActionWait(townie, 1, false));
		auto entrance = src->getEntrance()[0];
		townie->addActionFront(new dev::TownieActionWalk(townie, pathFinder->findPath(entrance->getX(), entrance->getY(), (int) townie->getX(), (int) townie->getY())));
	} else if(stage == 2) {
		if(!townie->getCarried()->transferTo(dst->getStorage())) {
			return dt;
		}
	} else if(stage == 3) {
		stage = -1;
	}
	stage++;
	return 0;
}

bool TownieActionCarry::isFinished()
{
	return stage == 4;
}

} // namespace dev
