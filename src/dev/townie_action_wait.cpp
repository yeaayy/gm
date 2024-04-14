#include "dev/townie_action_wait.hpp"

namespace dev {

TownieActionWait::TownieActionWait(Townie *townie, float time, bool hide)
	: TownieAction(townie)
{
	this->time = time;
	this->hide = hide;
}

float TownieActionWait::update(float dt)
{
	if(dt < time) {
		time -= dt;
		return dt;
	} else {
		dt = time;
		time = 0;
		return dt;
	}
}

bool TownieActionWait::isFinished()
{
	townie->setVisible(!hide || time <= 0);
	return time <= 0;
}

} // namespace dev
