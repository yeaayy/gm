#ifndef GAME_TOWNIE_HPP
#define GAME_TOWNIE_HPP

#include "state.hpp"
#include "updatable.hpp"
#include "linked_list.hpp"

class Camera;
class AbsHouse;
class Storage;
class TownieAction;
class JobDesc;
class WorldData;
enum class ResourceType;

extern const StateGroup *TownieStateDirection;
extern const StateGroup *TownieStateAction;
extern const StateGroup *TownieStateJob;
extern const StateGroup *TownieStateCarrying;

enum TownieActionState {
	TOWNIE_ACTION_STAND,
	TOWNIE_ACTION_IDLE,
	TOWNIE_ACTION_WALKING,
	TOWNIE_ACTION_WORKING,
};

enum TownieJobState {
	TOWNIE_JOBLESS,
	TOWNIE_HAS_JOB,
};

enum class Gender {
	MALE,
	FEMALE,
};

class Townie : public Updatable, public State
{
public:
	Townie(Gender, int variant, float x, float y, float z);
	virtual ~Townie();
	bool setHouse(AbsHouse *home);
	AbsHouse *getHouse();
	bool detachFromHouse();
	void update(float dt) override;
	void setJob(JobDesc *job);
	JobDesc *getJob();
	float getX();
	float getY();
	float getZ();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setPosition(float x, float y, float z);
	Storage *getCarried();
	ResourceType getCarriedType();
	bool isVisible();
	void setVisible(bool visible = true);
	void onPlaced(WorldData*);
	void onRemoved(WorldData*);

	bool hasAction();
	TownieAction *getFirstAction();
	TownieAction *getLastAction();
	void addActionFront(TownieAction *action);
	void addActionBack(TownieAction *action);

	static Storage *createStorage();

public:
	const Gender gender;
	const int variant;

private:
	float x, y, z;
	bool visible;
	JobDesc *job;
	AbsHouse *home;
	Storage *carried;
	ResourceType carriedType;
	LinkedList<TownieAction*> actionQueue;
};

class TownieAction
{
	friend Townie;
public:
	TownieAction(Townie *townie);
	virtual ~TownieAction();
	virtual void onStartAction();
	virtual void onEndAction();
	virtual float update(float dt) = 0;
	virtual bool isFinished() = 0;

protected:
	Townie *townie;
	bool started = false;
};

#endif /* GAME_TOWNIE_HPP */
