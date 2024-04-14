#include "building/small_house.hpp"

SmallHouse::SmallHouse(int x, int y)
	: AbsHouse(2, BUILDING_HOUSE_SMALL_1, x, y, 2, 2)
{
	addEntrance(2, 1, Dir::up_right);
}
