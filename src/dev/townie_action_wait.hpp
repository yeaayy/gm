#ifndef DEV_TOWNIE_ACTION_WAIT_HPP
#define DEV_TOWNIE_ACTION_WAIT_HPP

#include "game/townie.hpp"

namespace dev {

class TownieActionWait : public TownieAction
{
  public:
	TownieActionWait(Townie *townie, float time, bool hide);
	float update(float dt) override;
	bool isFinished() override;

  private:
	float time;
	bool hide;
};

} // namespace dev

#endif /* DEV_TOWNIE_ACTION_WAIT_HPP */
