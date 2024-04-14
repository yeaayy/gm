#include "direction.hpp"

#include <math.h>

Dir::Dir(Direction _id, int _dx, int _dy, float _dist)
    : id(_id), dx(_dx), dy(_dy), dist(_dist) {}

const Dir *Dir::unknown    = new Dir(Direction::UNKNOWN   ,  0,  0, 0);
const Dir *Dir::up         = new Dir(Direction::UP        , -1, -1, M_SQRT2);
const Dir *Dir::up_left    = new Dir(Direction::UP_LEFT   ,  0, -1, 1);
const Dir *Dir::left       = new Dir(Direction::LEFT      ,  1, -1, M_SQRT2);
const Dir *Dir::down_left  = new Dir(Direction::DOWN_LEFT ,  1,  0, 1);
const Dir *Dir::down       = new Dir(Direction::DOWN      ,  1,  1, M_SQRT2);
const Dir *Dir::down_right = new Dir(Direction::DOWN_RIGHT,  0,  1, 1);
const Dir *Dir::right      = new Dir(Direction::RIGHT     , -1,  1, M_SQRT2);
const Dir *Dir::up_right   = new Dir(Direction::UP_RIGHT  , -1,  0, 1);

void Dir::apply(int *x, int *y)
const {
    *x += this->dx;
    *y += this->dy;
}

const Dir *Dir::getDir(int sx, int sy, int ex, int ey)
{
    switch(sx - ex) {
    case -1:
        switch(sy - ey) {
        case -1:
            return Dir::up;
        case 0:
            return Dir::up_right;
        case 1:
            return Dir::right;
        }
        break;
    case 0:
        switch(sy - ey) {
        case -1:
            return Dir::up_left;
        case 1:
            return Dir::down_right;
        }
        break;
    case 1:
        switch(sy - ey) {
        case -1:
            return Dir::left;
        case 0:
            return Dir::down_left;
        case 1:
            return Dir::down;
        }
        break;
    }
    return Dir::unknown;
}

