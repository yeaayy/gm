#include "townie/action_complain.hpp"


TownieActionComplain::TownieActionComplain(Townie *townie, WorldData *world)
    : TownieAction(townie)
{
    this->time = 5;
}

void TownieActionComplain::onStartAction()
{
    townie->setState(TownieStateDirection, 4);
    townie->setState(TownieStateAction, TOWNIE_ACTION_STAND);
}

float TownieActionComplain::update(float dt)
{
    if(time > dt) {
        time -= dt;
        return dt;
    } else {
        float timeLeft = time;
        time = 0;
        return timeLeft;
    }
}

bool TownieActionComplain::isFinished()
{
    return time <= 0;
}

