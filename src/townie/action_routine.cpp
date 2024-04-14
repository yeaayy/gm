#include "townie/action_routine.hpp"

#include "building/production.hpp"
#include "townie/action_start_work.hpp"
#include "townie/action_rest.hpp"
#include "townie/action_wait.hpp"

TownieActionRoutine::TownieActionRoutine(Townie *townie, WorldData *world)
    : TownieAction(townie)
{
    this->world = world;
}

void TownieActionRoutine::onStartAction()
{
    if(townie->gender == Gender::MALE) {
        townie->addActionBack(new TownieActionStartWork(townie, world));
    } else {
        townie->addActionBack(new TownieActionWait(townie, 5));
    }
    townie->addActionBack(new TownieActionRest(townie, world));
    townie->addActionBack(new TownieActionRoutine(townie, world));
}

float TownieActionRoutine::update(float)
{
    return 0;
}

bool TownieActionRoutine::isFinished()
{
    return true;
}

