#ifndef TOWNIE_ACTION_HIDE_HPP
#define TOWNIE_ACTION_HIDE_HPP

#include "game/townie.hpp"

class TownieActionHide : public TownieAction
{
public:
    TownieActionHide(Townie *townie, float time);
    float update(float) override;
    bool isFinished() override;
	void onStartAction() override;
	void onEndAction() override;

private:
    float time;
};

#endif /* TOWNIE_ACTION_HIDE_HPP */
