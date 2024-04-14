#include "game/townie.hpp"

#include <math.h>

#include "building/abs_house.hpp"
#include "game/storage.hpp"
#include "townie/action_routine.hpp"

#include "list.tcc"
#include "linked_list.tcc"

namespace {
class OnCarriedChangeListener : public Storage::OnStorageChangeListener
{
public:
	OnCarriedChangeListener(Townie *_townie, ResourceType *_carriedType)
		: townie(_townie), carriedType(_carriedType) {}
	void onStorageChange(Storage *storage) override
	{
		*carriedType = storage->getFirstNonEmpty();
		townie->setState(TownieStateCarrying, *carriedType != ResourceType::UNKNOWN);
	}

private:
	Townie *townie;
	ResourceType *carriedType;
};
}

const StateGroup *TownieStateDirection = State::create(nullptr, 3);
const StateGroup *TownieStateAction = State::create(TownieStateDirection, 2);
const StateGroup *TownieStateJob = State::create(TownieStateAction, 1);
const StateGroup *TownieStateCarrying = State::create(TownieStateJob, 1);

Townie::Townie(Gender _gender, int _variant, float x, float y, float z)
  : Updatable(true), gender(_gender), variant(_variant)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->visible = true;
	this->home = nullptr;
	this->job = nullptr;
	this->carried = Townie::createStorage();
	this->carried->addOnStorageChangeListener(new OnCarriedChangeListener(this, &carriedType));
}

Townie::~Townie()
{
	delete carried;
}

Storage *Townie::createStorage()
{
	Storage *result = new Storage(1);
	result->setCapacity(ResourceType::WOOD, 5);
	return result;
}

void Townie::update(float dt)
{
	if(actionQueue.size() == 0) return;
	TownieAction *currentAction;
	do {
		currentAction = actionQueue.first();
		if(!currentAction->started) {
			currentAction->started = true;
			currentAction->onStartAction();
		}
		dt -= currentAction->update(dt);
		if(currentAction->isFinished()) {
			currentAction->onEndAction();
			delete currentAction;
			actionQueue.remove_item(currentAction);
		}
	} while(actionQueue.size() > 0 && dt > 0);
}

bool Townie::setHouse(AbsHouse *home)
{
	if(this->home != nullptr) {
		return false;
	}
	this->home = home;
	return home->put(this);
}

AbsHouse *Townie::getHouse()
{
	return home;
}

bool Townie::detachFromHouse()
{
	if(home == nullptr) {
		return true;
	}
	home = nullptr;
	return home->remove(this);
}

void Townie::setJob(JobDesc *job)
{
	this->job = job;
}

JobDesc *Townie::getJob()
{
	return this->job;
}

float Townie::getX()
{
	return x;
}

float Townie::getY()
{
	return y;
}

float Townie::getZ()
{
	return z;
}

void Townie::setX(float x)
{
	this->x = x;
}

void Townie::setY(float y)
{
	this->y = y;
}

void Townie::setZ(float z)
{
	this->z = z;
}

void Townie::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Storage *Townie::getCarried()
{
	return carried;
}

ResourceType Townie::getCarriedType()
{
	return carriedType;
}

bool Townie::isVisible()
{
	return visible;
}

void Townie::setVisible(bool visible)
{
	this->visible = visible;
}

void Townie::onPlaced(WorldData *world)
{
	addActionBack(new TownieActionRoutine(this, world));
}

void Townie::onRemoved(WorldData *world)
{
}

bool Townie::hasAction()
{
	return actionQueue.size() > 0;
}

TownieAction *Townie::getFirstAction()
{
	return actionQueue.first();
}

TownieAction *Townie::getLastAction()
{
	return actionQueue.last();
}

void Townie::addActionFront(TownieAction *action)
{
	actionQueue.push_front(action);
}

void Townie::addActionBack(TownieAction *action)
{
	actionQueue.push_back(action);
}


TownieAction::TownieAction(Townie *townie)
{
	this->townie = townie;
}

void TownieAction::onStartAction()
{}

void TownieAction::onEndAction()
{}

TownieAction::~TownieAction()
{}
