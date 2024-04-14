#include "townie/action_work.hpp"


TownieActionWork::TownieActionWork(Townie *townie, float _duration, Direction _direction)
    : TownieAction(townie), duration(_duration), direction(_direction) {}

void TownieActionWork::onStartAction()
{
    townie->setState(TownieStateDirection, (int) direction);
    townie->setState(TownieStateAction, TOWNIE_ACTION_WORKING);
}

float TownieActionWork::update(float dt)
{
	if(dt < duration) {
		duration -= dt;
		return dt;
	} else {
		dt = duration;
		duration = 0;
		return dt;
	}
}

bool TownieActionWork::isFinished()
{
    return duration <= 0;
}


