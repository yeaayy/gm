#include "townie/action_pickup.hpp"

#include "game/storage.hpp"

TownieActionPickup::TownieActionPickup(Townie *townie, Storage *_src, Storage *_dst, bool _animate)
    : TownieAction(townie), animate(_animate), src(_src), dst(_dst) {}

void TownieActionPickup::onStartAction()
{
    src->transferToVia(townie->getCarried(), dst);
}

float TownieActionPickup::update(float dt)
{
    return 0;
}

bool TownieActionPickup::isFinished()
{
    return true;
}

