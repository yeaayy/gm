#include "townie/action_start_work.hpp"

#include "building/production.hpp"
#include "game/world_data.hpp"
#include "townie/action_walk.hpp"

TownieActionStartWork::TownieActionStartWork(Townie *townie, WorldData *world)
    : TownieAction(townie)
{
    this->world = world;
}

void TownieActionStartWork::onStartAction()
{
    JobDesc *job = townie->getJob();
    if(job != nullptr) {
        townie->addActionFront(job->createAction(townie, world));
        townie->addActionFront(new TownieActionWalk(world->getPathFinder(), townie, job->getJobSite()));
    }
}

float TownieActionStartWork::update(float)
{
    return 0;
}

bool TownieActionStartWork::isFinished()
{
    return true;
}

