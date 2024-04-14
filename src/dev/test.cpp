#include <stdio.h>

#include "game/townie.hpp"
#include "log.h"

/*
namespace {

enum class BuildingCategory {
    storage,
    production,
    job_site,
    natural_resource,
    house,
    deco,
    walkable_deco,
    // ruin?
};

enum BuildingType {
    // storage
    BUILDING_TYPE_CASTLE,
    BUILDING_TYPE_STORAGE,
    // production
    BUILDING_TYPE_FORESTER,
    BUILDING_TYPE_CARPENTER,
    // house
    BUILDING_TYPE_SMALL_HOUSE_1,
    BUILDING_TYPE_SMALL_HOUSE_2,
    BUILDING_TYPE_LARGE_HOUSE,
    // natural resource
    BUILDING_TYPE_FOREST,
    BUILDING_TYPE_WOOD,
    // deco
    BUILDING_TYPE_WALL,
    BUILDING_TYPE_STATUE,
    BUILDING_TYPE_FOUNTAIN,
    // walkable deco
    BUILDING_TYPE_STREET_1,
    BUILDING_TYPE_STREET_2,
};

class JobSite
{
public:
private:
    int level;
    int *slotsPerLevel;
    TownieAction *jobTypes;
}

class Building
{
public:
	Building(BuildingCategory, BuildingType, int x, int y, int width, int height);
	virtual ~Building();
	BuildingCategory getCategory() const;
	BuildingType getType() const;
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;

 private:
	int x, y;
	int width, height;
	const BuildingCategory category;
	const BuildingType type;
};

class Wood: public Building
{
public:
    int getAmount();
    int getCapacity();
};

Building *makeDeco(BuildingType type, int x, int y)
{
    switch(type) {
    case BUILDING_TYPE_WALL:
    case BUILDING_TYPE_STREET_1:
    case BUILDING_TYPE_STREET_2:
        return new Building(BuildingCategory::deco, type, x, y, 1, 1);
    default:
        return nullptr;
    }
}

}

int main()
{
    return 0;
}*/