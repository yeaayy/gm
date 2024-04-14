#include "townie/action_drop.hpp"

#include "building/building.hpp"
#include "game/storage.hpp"

TownieActionDrop::TownieActionDrop(Townie *townie, Storage *_dst, Direction _direction, bool _animate)
    : TownieAction(townie), animate(_animate), dst(_dst), direction(_direction) {}

void TownieActionDrop::onStartAction()
{
    townie->setState(TownieStateDirection, (int) direction);
    townie->getCarried()->transferTo(dst);
}

void TownieActionDrop::onEndAction()
{
    townie->setState(TownieStateAction, TOWNIE_ACTION_STAND);
}

float TownieActionDrop::update(float dt)
{
    return 0;
}

bool TownieActionDrop::isFinished()
{
    return true;
}

