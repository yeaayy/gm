#ifndef TOWNIE_CARRIER_WORK_HPP
#define TOWNIE_CARRIER_WORK_HPP

#include "timer.hpp"
#include "building/production.hpp"

class TownieActionCarrierWork: public TownieAction, public JobDesc, Timer::OnTimeout
{
public:
    TownieActionCarrierWork(BuildingProduction *storage);
	void onStartAction() override;
	void onEndAction() override;
	float update(float dt) override;
	bool isFinished() override;
    void onTimerTimeout(Timer *timer) override;
	TownieAction *createAction(Townie *townie, WorldData *world) override;

private:
    TownieActionCarrierWork(BuildingProduction *storage, Townie *townie, WorldData *);
    WorldData *world;
    Timer *timer;
    bool finished;
};

#endif /* TOWNIE_CARRIER_WORK_HPP */
