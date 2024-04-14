#include "game/enterable.hpp"

#include "list.tcc"

Entrance::Entrance(Enterable *_parent, int _relX, int _relY, const Dir *dir)
    : relX(_relX), relY(_relY), direction(dir), parent(_parent) {}

int Entrance::getX()
const {
    return parent->getX() + relX;
}

int Entrance::getY()
const {
    return parent->getY() + relY;
}

Enterable::Enterable(int type, int x, int y, int width, int height)
    : GameObject(type, x, y, width, height) {}

Enterable::~Enterable()
{}

const List<const Entrance*> &Enterable::getEntrance()
const {
    return entrances;
}

const Dir *Enterable::getDirection(int entranceX, int entranceY)
const {
    // TODO: Implement this
    return nullptr;
}

void Enterable::addEntrance(int relX, int relY, const Dir *dir)
{
    entrances.push(new Entrance(this, relX, relY, dir));
}

