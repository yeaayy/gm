#include "townie/action_wait.hpp"

TownieActionWait::TownieActionWait(Townie *townie, float time)
    : TownieAction(townie)
{
    this->time = time;
    this->townie = townie;
}

float TownieActionWait::update(float dt)
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

bool TownieActionWait::isFinished()
{
    return time <= 0;
}

void TownieActionWait::onStartAction()
{
    // townie->setState(TownieActionState, TOWNIE_ACTION_IDLE);
}
