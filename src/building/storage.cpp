#include "building/storage.hpp"

#include "game/storage.hpp"
#include "townie/carrier_work.hpp"

StorageBuilding::StorageBuilding(Storage *sharedStorage, int maxLevel, int *slotCount, int type, int x, int y, int width, int height)
	: BuildingProduction(type, x, y, width, height)
{
	this->storage = sharedStorage;
	this->maxLevel = maxLevel;
	JobDesc *jobCarrier = new TownieActionCarrierWork(this);
	JobDesc **pointer = new JobDesc*[2] {
        jobCarrier,
        nullptr
    };
	jobDesc = new JobDesc**[maxLevel];
	for(int i = 0; i < maxLevel; i++) {
		jobDesc[i] = pointer;
	}
}

StorageBuilding::~StorageBuilding()
{
    delete jobDesc[0][0];
    delete[] jobDesc[0];
}

void StorageBuilding::onPlaced(WorldData *dst)
{
	const int *capacity = getSpesificCapacity();
	for(int i=0; i<(int) ResourceType::COUNT; i++) {
		ResourceType type = (ResourceType) i;
		storage->setCapacity(type, storage->getCapacity(type) + capacity[i]);
	}
	GameObject::onPlaced(dst);
}
