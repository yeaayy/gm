#include "game/world_data.hpp"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

#include "pfa_simple.hpp"
#include "building/wood.hpp"
#include "game/object.hpp"
#include "game/townie.hpp"

#include "list.tcc"

WorldData::WorldData()
{
	width = 4;
	height = 4;
	elevation = nullptr;
	terrain = nullptr;
	path = nullptr;
	pfa = nullptr;
}

WorldData::~WorldData()
{
	if(elevation) delete[] elevation;
	if(terrain) delete[] terrain;
	if(path) delete path;
	if(pfa) delete pfa;
}

void WorldData::init(int width, int height)
{
	this->width = width;
	this->height = height;
	int count = height * width;
	elevation = new int[count];
	terrain = new TerrainType[count];
	for(int i=0; i<count; i++) {
		terrain[i] = TERRAIN_TYPE_GROUND;
		elevation[i] = 0;
	}
	fillFlag();
	pfa = new pfa::SimplePathFinder(this);
}

bool WorldData::load(const char *path)
{
	this->path = strdup(path);
	FILE *file = fopen(path, "rb");
	if(file == nullptr) return false;
	fread(&width, sizeof(int), 1, file);
	fread(&height, sizeof(int), 1, file);

	int count = height * width;
	elevation = new int[count];
	terrain = new TerrainType[count];

	fread(terrain, sizeof(*terrain) * count, 1, file);
	fread(elevation, sizeof(*elevation) * count, 1, file);

	fclose(file);
	fillFlag();
	pfa = new pfa::SimplePathFinder(this);
	return true;
}

bool WorldData::save()
{
	return saveAs(path);
}

bool WorldData::saveAs(const char *path)
{
	FILE *file = fopen(path, "wb");
	if(file == nullptr) return false;
	fwrite(&width, sizeof(int), 1, file);
	fwrite(&height, sizeof(int), 1, file);

	fwrite(terrain, sizeof(*terrain) * height * width, 1, file);
	fwrite(elevation, sizeof(*elevation) * height * width, 1, file);
	fclose(file);
	return true;
}

void WorldData::fillFlag()
{
	int count = (height - 1) * (width - 1);
	objectGrid = new GameObject*[count];
	for(int i=0; i<count; i++) {
		objectGrid[i] = nullptr;
	}

	flags = new TerrainFlag[count];
	for(int y=0; y<(height - 1); y++) {
		for(int x=0; x<(height - 1); x++) {
			updateFlag(x, y);
		}
	}
}

void WorldData::updateFlag(int x, int y)
{
	int i = y * width + x;
	TerrainType a = terrain[i];
	TerrainType b = terrain[i + 1];
	TerrainType c = terrain[i + width];
	TerrainType d = terrain[i + width +1];
	#define P(s, t, u, v) \
		(s + TERRAIN_TYPE_MAX * (t + TERRAIN_TYPE_MAX * (u + TERRAIN_TYPE_MAX * v)))
	#define G TERRAIN_TYPE_GROUND
	#define W TERRAIN_TYPE_WATER
	#define F TERRAIN_TYPE_FLOOD
	#define C TERRAIN_TYPE_CLIFF
	#define E TERRAIN_TYPE_EDGE
	TerrainFlag result;
	switch(P(a, b, c, d)) {
		case P(G, G, G, G): result = TERRAIN_FLAG_FLAT; break;
		case P(W, W, W, W): result = TERRAIN_FLAG_WATER; break;
		default: result = TERRAIN_FLAG_NONE;
	}
	#undef P
	flags[y * (width - 1) + x] = result;
}

TerrainFlag WorldData::getFlag(int x, int y)
const {
	if(objectGrid[y * (width - 1) + x] != nullptr) {
		return TERRAIN_FLAG_OBJECT;
	}
	return (TerrainFlag) (flags[y * (width - 1) + x] & TERRAIN_FLAG_MAX);
}

GameObject *WorldData::getObjectAt(int x, int y)
{
	if(isOutOfRange2(x, y)) return nullptr;
	return objectGrid[y * (width - 1) + x];
}

List<GameObject*> &WorldData::getObjects()
{
	return objects;
}

List<Townie*> &WorldData::getTownies()
{
	return townies;
}

TerrainType WorldData::getTerrain(int x, int y)
{
	if(isOutOfRange(x, y)) return TERRAIN_TYPE_GROUND;
	return terrain[y * width + x];
}

void WorldData::setTerrain(int x, int y, TerrainType type)
{
	if(isOutOfRange(x, y)) return;
	terrain[y * width + x] = type;
	if(!isOutOfRange(x - 1, y)) updateFlag(x - 1, y);
	if(!isOutOfRange(x, y - 1)) updateFlag(x, y - 1);
	if(!isOutOfRange(x - 1, y - 1)) updateFlag(x - 1, y - 1);
	updateFlag(x, y);
}

int WorldData::getElevation(int x, int y)
const {
	if(isOutOfRange(x, y)) return TERRAIN_TYPE_GROUND;
	return elevation[y * width + x];
}

void WorldData::setElevation(int x, int y, int elevation)
{
	if(isOutOfRange(x, y)) return;
	this->elevation[y * width + x] = elevation;
}

void WorldData::addElevation(int x, int y, int add)
{
	if(isOutOfRange(x, y)) return;
	this->elevation[y * width + x] += add;
}

float WorldData::getTerrainCost(int x, int y)
const {
	if(isOutOfRange(x, y)) return INFINITY;
	switch(getFlag(x, y)) {
		case TERRAIN_FLAG_FLAT:
			return 1;
		case TERRAIN_FLAG_OBJECT:
			switch(objectGrid[y * (width - 1) + x]->getType()) {
			case OBJECT_WOOD:
				return 1.5f;
			default:
				return INFINITY;
			}
		default:
			return INFINITY;
	}
}


int WorldData::getTerrainWidth()
const {
	return width;
}

int WorldData::getTerrainHeight()
const {
	return height;
}

int WorldData::getWidth()
const {
	return width - 1;
}

int WorldData::getHeight()
const {
	return height - 1;
}

bool WorldData::isOutOfRange(int x, int y)
const {
	return x < 0 || x >= width || y < 0 || y >= height;
}

bool WorldData::isOutOfRange2(int x, int y)
const {
	return x < 0 || x >= width - 1 || y < 0 || y >= height - 1;
}

void WorldData::put(GameObject *obj)
{
	obj->onPlaced(this);
	objects.push(obj);
	for(int i = 0; i < objectsListeners.size(); i++) {
		objectsListeners[i]->onGameObjectAdded(obj);
	}
}

void WorldData::remove(GameObject *obj)
{
	obj->onRemoved(this);
	assert_true(objects.removeItem(obj));
	for(int i = 0; i < objectsListeners.size(); i++) {
		objectsListeners[i]->onGameObjectRemoved(obj);
	}
}

void WorldData::put(Townie *obj)
{
	obj->onPlaced(this);
	townies.push(obj);
	for(int i = 0; i < towniesListeners.size(); i++) {
		towniesListeners[i]->onTownieAdded(obj);
	}
}

void WorldData::remove(Townie *obj)
{
	obj->onRemoved(this);
	assert_true(townies.removeItem(obj));
	for(int i = 0; i < towniesListeners.size(); i++) {
		towniesListeners[i]->onTownieRemoved(obj);
	}
}

void WorldData::addGameObjectListener(WorldData::GameObjectListener *listener)
{
	objectsListeners.push(listener);
	for(int i=0; i<objects.size(); i++) {
		listener->onGameObjectAdded(objects[i]);
	}
}

void WorldData::removeGameObjectListener(WorldData::GameObjectListener *listener)
{
	objectsListeners.removeItem(listener);
}

void WorldData::addTownieListener(WorldData::TownieListener *listener)
{
	towniesListeners.push(listener);
	for(int i=0; i<townies.size(); i++) {
		listener->onTownieAdded(townies[i]);
	}
}

void WorldData::removeTownieListener(WorldData::TownieListener *listener)
{
	towniesListeners.removeItem(listener);
}

pfa::PathFinder *WorldData::getPathFinder()
{
	return pfa;
}
