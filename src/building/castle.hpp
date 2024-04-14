#ifndef BUILDING_CASTLE_HPP
#define BUILDING_CASTLE_HPP

#include "building/storage.hpp"

class Castle : public StorageBuilding
{
public:
	Castle(Storage *sharedStorage, int x, int y);
	const int *getSpesificCapacity() override;

protected:
	Storage *sharedStorage;
};

#endif /* BUILDING_CASTLE_HPP */
