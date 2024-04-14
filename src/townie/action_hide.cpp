#include "townie/action_hide.hpp"

TownieActionHide::TownieActionHide(Townie *townie, float time)
    : TownieAction(townie)
{
    this->time = time;
    this->townie = townie;
}

float TownieActionHide::update(float dt)
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

bool TownieActionHide::isFinished()
{
    return time <= 0;
}

void TownieActionHide::onStartAction()
{
    townie->setVisible(false);
}

void TownieActionHide::onEndAction()
{
    townie->setVisible(true);
}

