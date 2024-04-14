#ifndef BUILDING_PRODUCTION_HPP
#define BUILDING_PRODUCTION_HPP

#include "game/townie.hpp"
#include "building/building.hpp"

#define JOB_TYPE_CARRIER				0x4D78779B
#define JOB_TYPE_LUMBERJACK				0xF216D73F

class WorldData;
class BuildingProduction;

class JobDesc
{
public:
	JobDesc(BuildingProduction *jobSite, int typeId);
	virtual ~JobDesc();
	int getTypeId() const;
	BuildingProduction *getJobSite();
	virtual TownieAction *createAction(Townie *townie, WorldData *world) = 0;

private:
	int typeId;
	BuildingProduction *jobSite;
};

class BuildingProduction : public Building
{
public:
	BuildingProduction(int type, int x, int y, int width, int height);
	~BuildingProduction();
	int getSlotCount() const;
	JobDesc **getJobDescs();

protected:
	int *slotCount;
	JobDesc ***jobDesc;
};

#endif /* BUILDING_PRODUCTION_HPP */