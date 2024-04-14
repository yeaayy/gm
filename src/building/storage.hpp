#ifndef BUILDING_STORAGE_HPP
#define BUILDING_STORAGE_HPP

#include "building/production.hpp"

class StorageBuilding : public BuildingProduction
{
  public:
	StorageBuilding(Storage *sharedStorage, int maxLevel, int *slotCount, int type, int x, int y, int width, int height);
	virtual ~StorageBuilding();
	virtual const int *getSpesificCapacity() = 0;
	virtual void onPlaced(WorldData *dst);
};

#endif /* BUILDING_STORAGE_HPP */