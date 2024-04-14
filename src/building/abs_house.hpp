#ifndef BUILDING_ABS_HOUSE_HPP
#define BUILDING_ABS_HOUSE_HPP

#include "building/building.hpp"
#include "game/storage.hpp"
#include "list.hpp"

class Townie;

class AbsHouse : public Building
{
public:
	AbsHouse(int capacity, int type, int x, int y, int width, int height);
	virtual bool put(Townie *townie);
	virtual bool remove(Townie *townie);
	int getCapacity() const;
	List<Townie*> &getTownies() const;

private:
	int capacity;
	List<Townie*> townies;
};

#endif /* BUILDING_ABS_HOUSE_HPP */