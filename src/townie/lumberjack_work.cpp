#include "townie/lumberjack_work.hpp"

#include <math.h>

#include "building/wood.hpp"
#include "game/world_data.hpp"
#include "game/storage.hpp"
#include "townie/action_complain.hpp"
#include "townie/action_drop.hpp"
#include "townie/action_pickup.hpp"
#include "townie/action_work.hpp"
#include "townie/action_walk.hpp"

TownieActionLumberjackWork::TownieActionLumberjackWork(BuildingProduction *workSite)
    : TownieAction(nullptr), JobDesc(workSite, JOB_TYPE_LUMBERJACK)
{
    this->pfa = nullptr;
    this->world = nullptr;
    this->finished = false;
    this->timer = nullptr;
}

TownieActionLumberjackWork::TownieActionLumberjackWork(BuildingProduction *workSite, Townie *townie, pfa::PathFinder *pfa, WorldData *world)
    : TownieAction(townie), JobDesc(workSite, JOB_TYPE_LUMBERJACK)
{
    this->pfa = pfa;
    this->world = world;
    this->finished = false;
    this->timer = new Timer(60, this);
}

void TownieActionLumberjackWork::onStartAction()
{
    if(finished) return;
    timer->start();
    started = false;
    if(getJobSite()->getStorage()->isFull(ResourceType::WOOD)) {
        townie->addActionFront(new TownieActionComplain(townie, world));
        return;
    }
    List<GameObject*> &objects = world->getObjects();
    Wood *nearestWood = nullptr;
    float nearestDist = INFINITY;
    for(int i = 0; i < objects.size(); i++) {
        GameObject *object = objects[i];
        Wood *wood = dynamic_cast<Wood*>(object);
        if(wood == nullptr)
            continue;
        if(wood->getAmount(ResourceType::WOOD) == 0)
            continue;
        float dist = hypotf(townie->getX() - wood->getX() - wood->getWidth() / 2.0f, townie->getY() - wood->getY() - wood->getHeight() / 2.0f);
        if(dist < nearestDist) {
            nearestDist = dist;
            nearestWood = wood;
        }
    }
    if(nearestWood == nullptr) {
        townie->addActionFront(new TownieActionComplain(townie, world));
        return;
    }
    townie->addActionFront(new TownieActionDrop(townie, getJobSite()->getStorage(), Direction::UP_RIGHT, true));
    townie->addActionFront(new TownieActionWalk(pfa, townie, getJobSite()));
    townie->addActionFront(new TownieActionPickup(townie, nearestWood, getJobSite()->getStorage(), false));
    townie->addActionFront(new TownieActionWork(townie, 10, Direction::UP_LEFT));
    townie->addActionFront(new TownieActionWalk(pfa, townie, nearestWood));
}

void TownieActionLumberjackWork::onEndAction()
{}

float TownieActionLumberjackWork::update(float dt)
{
    return 0;
}

bool TownieActionLumberjackWork::isFinished()
{
   return finished;
}

void TownieActionLumberjackWork::onTimerTimeout(Timer *timer)
{
   finished = true;
   delete timer;
}

TownieAction *TownieActionLumberjackWork::createAction(Townie *townie, WorldData *world)
{
    return new TownieActionLumberjackWork(getJobSite(), townie, world->getPathFinder(), world);
}
