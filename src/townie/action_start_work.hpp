#ifndef TOWNIE_ACTION_START_WORK_HPP
#define TOWNIE_ACTION_START_WORK_HPP

#include "game/townie.hpp"

class WorldData;

class TownieActionStartWork : public TownieAction
{
public:
    TownieActionStartWork(Townie *townie, WorldData *world);
    float update(float) override;
    bool isFinished() override;
	void onStartAction() override;

private:
    WorldData *world;
};

#endif /* TOWNIE_ACTION_START_WORK_HPP */
