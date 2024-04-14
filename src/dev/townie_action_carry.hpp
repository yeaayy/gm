#ifndef DEV_TOWNIE_ACTION_CARRY_HPP
#define DEV_TOWNIE_ACTION_CARRY_HPP

#include "building/building.hpp"
#include "game/storage.hpp"
#include "game/townie.hpp"
#include "path_finder.hpp"

namespace dev {

class TownieActionCarry : public TownieAction
{
  public:
	TownieActionCarry(Townie*, pfa::PathFinder*, Building *src, Building *dst);
	float update(float dt) override;
	bool isFinished() override;

  private:
	int stage;
	pfa::PathFinder *pathFinder;
	Building *src;
	Building *dst;
};

} // namespace dev

#endif /* DEV_TOWNIE_ACTION_CARRY_HPP */
