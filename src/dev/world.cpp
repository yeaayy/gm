#include "dev/world.hpp"

#include <math.h>

#include "building/wood.hpp"
// #include "building/.hpp"
#include "game/world_data.hpp"

namespace dev {

World::World(WorldData *data)
{
	this->data = data;
}

float World::getTerrainCost(int x, int y)
const {
	if(x < 0 || x >= getWidth()) return INFINITY;
	if(y < 0 || y >= getHeight()) return INFINITY;
	switch(data->getFlag(x, y)) {
		case TERRAIN_FLAG_FLAT:
			return 1;
		case TERRAIN_FLAG_OBJECT:
			if(dynamic_cast<Wood*>(data->getObjectAt(x, y)) != nullptr) {
				return 1.5f;
			} else {
				return INFINITY;	
			}
		default:
			return INFINITY;
	}
}

int World::getElevation(int x, int y)
const {
	return data->getElevation(x, y);
}

int World::getWidth()
const {
	return data->getWidth();
}

int World::getHeight()
const {
	return data->getHeight();
}

} // namespace dev