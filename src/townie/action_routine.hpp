#ifndef TOWNIE_ACTION_ROUTINE_HPP
#define TOWNIE_ACTION_ROUTINE_HPP

#include "game/townie.hpp"

class TownieActionRoutine : public TownieAction
{
public:
    TownieActionRoutine(Townie *townie, WorldData *world);
    float update(float) override;
    bool isFinished() override;
	void onStartAction() override;

private:
    WorldData *world;
};

#endif /* TOWNIE_ACTION_ROUTINE_HPP */
