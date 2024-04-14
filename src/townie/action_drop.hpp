#ifndef TOWNIE_ACTION_DROP_HPP
#define TOWNIE_ACTION_DROP_HPP

#include "direction.hpp"
#include "game/townie.hpp"

class Storage;

class TownieActionDrop : public TownieAction
{
public:
	TownieActionDrop(Townie *townie, Storage *dst, Direction direction, bool animate);
	void onStartAction() override;
	void onEndAction() override;
	float update(float dt) override;
	bool isFinished() override;

private:
    bool animate;
    Storage *dst;
    Direction direction;
};

#endif /* TOWNIE_ACTION_DROP_HPP */
