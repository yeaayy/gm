#include "building/wood.hpp"

#include "game/storage.hpp"

Wood::Wood(int initialAmount, int capacity, int x, int y)
	: Enterable(OBJECT_WOOD, x, y, 2, 2)
	, Storage(1)
{
	setCapacity(ResourceType::WOOD, initialAmount == -1 ? capacity : initialAmount);
	addAmount(ResourceType::WOOD, capacity);
	addEntrance(1, 1, Dir::up_left);
}
