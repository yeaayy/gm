#include "building/castle.hpp"

#include "texture/sprite.hpp"

Castle::Castle(Storage *sharedStorage, int x, int y)
	: StorageBuilding(sharedStorage, 1, new int[1]{4}, BUILDING_CASTLE, x, y, 6, 5)
{
	addEntrance(6, 2, Dir::up_right);
}

const int *Castle::getSpesificCapacity()
{
	static int result[] {
		100,
		100,
		100,
	};
	return result;
}
