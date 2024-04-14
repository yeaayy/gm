#include "townie/carrier_work.hpp"

#include <math.h>

#include "building/storage.hpp"
#include "game/world_data.hpp"
#include "game/storage.hpp"
#include "townie/action_complain.hpp"
#include "townie/action_drop.hpp"
#include "townie/action_pickup.hpp"
#include "townie/action_wait.hpp"
#include "townie/action_walk.hpp"

TownieActionCarrierWork::TownieActionCarrierWork(BuildingProduction *workSite)
    : TownieAction(nullptr), JobDesc(workSite, JOB_TYPE_CARRIER)
{
    this->world = nullptr;
    this->finished = false;
    this->timer = nullptr;
}

TownieActionCarrierWork::TownieActionCarrierWork(BuildingProduction *workSite, Townie *townie, WorldData *world)
    : TownieAction(townie), JobDesc(workSite, JOB_TYPE_CARRIER)
{
    this->world = world;
    this->finished = false;
    this->timer = new Timer(60, this);
}

void TownieActionCarrierWork::onStartAction()
{
    if(finished) return;
    timer->start();
    started = false;
    // if(getJobSite()->getStorage()->isFull(ResourceType::WOOD)) {
    //     townie->addActionFront(new TownieActionComplain(townie, world));
    //     return;
    // }
    List<GameObject*> &objects = world->getObjects();
    BuildingProduction *source = nullptr; 
    Storage *dst = getJobSite()->getStorage();
    bool allEmpty = true;
    float nearestDist = INFINITY;
    for(int i = 0; i < objects.size(); i++) {
        GameObject *object = objects[i];
        StorageBuilding *storage = dynamic_cast<StorageBuilding*>(object);
        if(storage != nullptr) continue;
        BuildingProduction *prod = dynamic_cast<BuildingProduction*>(object);
        if(prod == nullptr) continue;
        Storage *buildingStorage = prod->getStorage();
        allEmpty &= buildingStorage->isEmpty();
        if(!buildingStorage->canTransferToVia(townie->getCarried(), dst))
            continue;
        float dist = hypotf(townie->getX() - prod->getX() - prod->getWidth() / 2.0f, townie->getY() - prod->getY() - prod->getHeight() / 2.0f);
        if(dist < nearestDist) {
            nearestDist = dist;
            source = prod;
        }
    }
    if(source == nullptr) {
        if(allEmpty) {
            // Just wait
            townie->addActionFront(new TownieActionWait(townie, 5.0f));
        } else {
            // Storage full
            townie->addActionFront(new TownieActionComplain(townie, world));
        }
        return;
    }
    pfa::PathFinder *pfa = world->getPathFinder();
    townie->addActionFront(new TownieActionDrop(townie, getJobSite()->getStorage(), Direction::UP_RIGHT, true));
    townie->addActionFront(new TownieActionWalk(pfa, townie, getJobSite()));
    townie->addActionFront(new TownieActionPickup(townie, source->getStorage(), getJobSite()->getStorage(), true));
    townie->addActionFront(new TownieActionWalk(pfa, townie, source));
}

void TownieActionCarrierWork::onEndAction()
{}

float TownieActionCarrierWork::update(float dt)
{
    return 0;
}

bool TownieActionCarrierWork::isFinished()
{
   return finished;
}

void TownieActionCarrierWork::onTimerTimeout(Timer *timer)
{
   finished = true;
   delete timer;
}

TownieAction *TownieActionCarrierWork::createAction(Townie *townie, WorldData *world)
{
    return new TownieActionCarrierWork(getJobSite(), townie, world);
}
