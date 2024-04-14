#ifndef TOWNIE_ACTION_PICKUP_HPP
#define TOWNIE_ACTION_PICKUP_HPP

#include "game/townie.hpp"

class Storage;

class TownieActionPickup : public TownieAction
{
public:
	TownieActionPickup(Townie *townie, Storage *src, Storage *dst, bool animate);
	void onStartAction() override;
	float update(float dt) override;
	bool isFinished() override;

private:
    bool animate;
    Storage *src;
    Storage *dst;
};

#endif /* TOWNIE_ACTION_PICKUP_HPP */
