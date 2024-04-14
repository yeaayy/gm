#ifndef DEV_TOWNIE_ACTION_WALK_HPP
#define DEV_TOWNIE_ACTION_WALK_HPP

#include "game/townie.hpp"
#include "path_finder.hpp"

#include "linked_list.hpp"

class WorldData;

namespace dev {

class TownieActionWalk : public TownieAction
{
public:
	TownieActionWalk(Townie *townie, const pfa::Point *rpath);
	~TownieActionWalk();
	void onStartAction() override;
	void onEndAction() override;
	float update(float dt) override;
	bool isFinished() override;

private:
	LinkedList<pfa::Point*> path;
	int direction;
};

} // namespace dev

#endif /* DEV_TOWNIE_ACTION_WALK_HPP */
