#include "building/building.hpp"

Building::Building(int type, int x, int y, int width, int height)
	: Enterable(type, x, y, width, height)
{
	level = 0;
	maxLevel = 1;
	storage = nullptr;
}

Building::~Building()
{
}

void Building::setLevel(int)
{}

int Building::getLevel()
const {
	return level;
}

int Building::getMaxLevel()
const {
	return maxLevel;
}

Storage *Building::getStorage()
{
	return storage;
}
