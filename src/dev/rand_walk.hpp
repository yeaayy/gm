#ifndef DEV_RAND_WALK_HPP
#define DEV_RAND_WALK_HPP

#include "game/townie.hpp"
#include "linked_list.hpp"

class WorldData;
namespace pfa {
class SimplePathFinder;
class Point;
}

namespace dev {
class TownieActionRandomWalk : public TownieAction
{
public:
	TownieActionRandomWalk(Townie *townie, WorldData *world, pfa::SimplePathFinder *spf);
	~TownieActionRandomWalk();
	float update(float dt) override;
	bool isFinished() override;

private:
	LinkedList<pfa::Point*> path;
	WorldData *world;
	pfa::SimplePathFinder *spf;
};
} // namespace dev

#endif /* DEV_RAND_WALK_HPP */
