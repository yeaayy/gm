#ifndef TOWNIE_ACTION_WAIT_HPP
#define TOWNIE_ACTION_WAIT_HPP

#include "game/townie.hpp"

class TownieActionWait : public TownieAction
{
public:
    TownieActionWait(Townie *townie, float time);
    float update(float) override;
    bool isFinished() override;
	void onStartAction() override;

private:
    float time;
};

#endif /* TOWNIE_ACTION_WAIT_HPP */
