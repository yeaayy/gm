#ifndef BUILDING_WOOD_HPP
#define BUILDING_WOOD_HPP

#include "game/enterable.hpp"
#include "game/object.hpp"
#include "game/storage.hpp"

#define OBJECT_WOOD						0x6EA52E15

class Wood : public Enterable, public Storage
{
public:
	Wood(int initialAmound, int capacity, int x, int y);
};

#endif /* BUILDING_WOOD_HPP */