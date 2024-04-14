#ifndef TOWNIE_ACTION_WORK_HPP
#define TOWNIE_ACTION_WORK_HPP

#include "direction.hpp"
#include "game/townie.hpp"

class TownieActionWork: public TownieAction
{
public:
    TownieActionWork(Townie *townie, float duration, Direction direction);
	void onStartAction() override;
	float update(float dt) override;
	bool isFinished() override;
private:
    float duration;
    TownieActionState endState;
    Direction direction;
};

#endif /* TOWNIE_ACTION_WORK_HPP */
