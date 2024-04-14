#include "townie/action_rest.hpp"

#include "building/abs_house.hpp"
#include "game/world_data.hpp"
#include "townie/action_walk.hpp"
#include "townie/action_hide.hpp"

TownieActionRest::TownieActionRest(Townie *townie, WorldData *world)
    : TownieAction(townie)
{
    this->world = world;
}

void TownieActionRest::onStartAction()
{
    townie->addActionFront(new TownieActionHide(townie, 10));
    townie->addActionFront(new TownieActionWalk(world->getPathFinder(), townie, townie->getHouse()));
}

float TownieActionRest::update(float)
{
    return 0;
}

bool TownieActionRest::isFinished()
{
    return true;
}

