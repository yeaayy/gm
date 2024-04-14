#ifndef TOWNIE_ACTION_COMPLAIN_HPP
#define TOWNIE_ACTION_COMPLAIN_HPP

#include "game/townie.hpp"

class WorldData;

class TownieActionComplain : public TownieAction
{
public:
    TownieActionComplain(Townie *townie, WorldData *world);
	void onStartAction() override;
    float update(float dt) override;
    bool isFinished() override;

private:
    float time;
};

#endif /* TOWNIE_ACTION_COMPLAIN_HPP */
