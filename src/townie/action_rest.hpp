#ifndef TOWNIE_ACTION_REST_HPP
#define TOWNIE_ACTION_REST_HPP

#include "game/townie.hpp"

class TownieActionRest : public TownieAction
{
public:
    TownieActionRest(Townie *townie, WorldData *world);
    float update(float) override;
    bool isFinished() override;
	void onStartAction() override;

private:
    float time;
    WorldData *world;
};

#endif /* TOWNIE_ACTION_REST_HPP */
