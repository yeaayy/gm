#include "building/abs_house.hpp"

#include "game/townie.hpp"
#include "list.tcc"

AbsHouse::AbsHouse(int _capacity, int type, int x, int y, int width, int height)
	: Building(type, x, y, width, height), capacity(_capacity)
{}

bool AbsHouse::put(Townie *townie)
{
	if(townies.size() >= capacity) {
		return false;
	}
	townies.push(townie);
	return true;
}

bool AbsHouse::remove(Townie *townie)
{
	return townies.removeItem(townie);
}
