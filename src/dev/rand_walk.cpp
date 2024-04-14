#include "dev/rand_walk.hpp"

#include <stdlib.h>

#include "building/abs_house.hpp"
#include "dev/townie_action_walk.hpp"
#include "dev/townie_action_wait.hpp"
#include "game/world_data.hpp"
#include "pfa_simple.hpp"

#include "log.h"
#include "linked_list.tcc"

namespace dev {

TownieActionRandomWalk::TownieActionRandomWalk(Townie *townie, WorldData *world, pfa::SimplePathFinder *spf)
	: TownieAction(townie)
{
	this->world = world;
	this->spf = spf;
}

TownieActionRandomWalk::~TownieActionRandomWalk()
{
	while(!path.isEmpty()) {
		pfa::Point *curr = path.remove_front();
		delete curr;
	}
}

float TownieActionRandomWalk::update(float dt)
{
	int x = (int) townie->getX() + rand() % 20 - 10;
	int y = (int) townie->getY() + rand() % 20 - 10;
	bool hide = false;
	if(rand() % 2 == 0 && townie->getHouse() != nullptr) {
		auto entrance = townie->getHouse()->getEntrance()[0];
		x = entrance->getX();
		y = entrance->getY();
		hide = true;
	}
	const pfa::Point *rpath = spf->findPath(x, y, (int) townie->getX(), (int) townie->getY());
	if(rpath == nullptr) return dt;
	townie->addActionFront(new dev::TownieActionWait(townie, 2 + (float)(rand()%1000)/1000.0f, hide));
	townie->addActionFront(new dev::TownieActionWalk(townie, rpath));
	return dt;
}

bool TownieActionRandomWalk::isFinished()
{
	return false;
}

} // namespace dev
