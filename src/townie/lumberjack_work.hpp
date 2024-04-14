#ifndef TOWNIE_LUMBERJACK_WORK_HPP
#define TOWNIE_LUMBERJACK_WORK_HPP

#include "timer.hpp"
#include "building/production.hpp"
#include "game/townie.hpp"

class WorldData;
namespace pfa {
class PathFinder;
}

class TownieActionLumberjackWork: public TownieAction, public JobDesc, Timer::OnTimeout
{
public:
    TownieActionLumberjackWork(BuildingProduction *lumberjack);
	void onStartAction() override;
	void onEndAction() override;
	float update(float dt) override;
	bool isFinished() override;
    void onTimerTimeout(Timer *timer) override;
	TownieAction *createAction(Townie *townie, WorldData *world) override;

private:
    TownieActionLumberjackWork(BuildingProduction *lumberjack, Townie *townie, pfa::PathFinder *, WorldData *);
    pfa::PathFinder *pfa;
    WorldData *world;
    Timer *timer;
    bool finished;
};

#endif /* TOWNIE_LUMBERJACK_WORK_HPP */
