#ifndef TOWNIE_ACTION_WALK_HPP
#define TOWNIE_ACTION_WALK_HPP

#include "linked_list.hpp"
#include "path_finder.hpp"
#include "game/townie.hpp"

class Enterable;

class TownieActionWalk : public TownieAction
{
public:
	TownieActionWalk(pfa::PathFinder *pathFinder, Townie *townie, int dstX, int dstY);
	TownieActionWalk(pfa::PathFinder *pathFinder, Townie *townie, const Enterable *dst);
	~TownieActionWalk();
	void onStartAction() override;
	void onEndAction() override;
	float update(float dt) override;
	bool isFinished() override;

private:
    pfa::PathFinder *pathFinder;
    int dstX, dstY;
    const Enterable *dst;
	LinkedList<pfa::Point*> path;
	int direction;
};

#endif /* TOWNIE_ACTION_WALK_HPP */
