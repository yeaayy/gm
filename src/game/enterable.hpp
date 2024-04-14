#ifndef GAME_ENTERABLE_HPP
#define GAME_ENTERABLE_HPP

#include "game/object.hpp"
#include "direction.hpp"
#include "list.hpp"

class Enterable;

class Entrance
{
    friend Enterable;
public:
	int getX() const;
	int getY() const;

	const int relX, relY;
    const Dir *direction;
private:
	Entrance(Enterable *parent, int relX, int relY, const Dir *dir);
	Enterable *parent;
};

class Enterable : public GameObject
{
public:
    Enterable(int type, int x, int y, int width, int height);
    virtual ~Enterable();
    const List<const Entrance*> &getEntrance() const;
    const Dir *getDirection(int entranceX, int entranceY) const;

protected:
    void addEntrance(int relX, int relY, const Dir *dir);
    List<const Entrance*> entrances;
};

#endif /* GAME_ENTERABLE_HPP */
