
#ifndef BUILDING_BUILDING_HPP
#define BUILDING_BUILDING_HPP

#include "game/enterable.hpp"
#include "game/object.hpp"

#define BUILDING_CASTLE					0x76E6B7D2
#define BUILDING_HOUSE_SMALL_1			0x7F9FA08D
#define BUILDING_LUMBERJACK				0x44C46FD0

class Storage;

class Building : public Enterable
{
public:
	Building(int type, int x, int y, int width, int height);
	virtual ~Building();
	void setLevel(int);
	int getLevel() const;
	int getMaxLevel() const;
	Storage *getStorage();

protected:
	int maxLevel;
	int level;
	Storage *storage;
};

#endif /* BUILDING_BUILDING_HPP */
