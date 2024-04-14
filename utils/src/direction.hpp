#ifndef UTILS_DIRECTION_H
#define UTILS_DIRECTION_H

enum class Direction {
    UP,
    UP_LEFT,
    LEFT,
    DOWN_LEFT,
    DOWN,
    DOWN_RIGHT,
    RIGHT,
    UP_RIGHT,
    UNKNOWN,
};

class Dir {
public:
    static const Dir *unknown;
    static const Dir *down_left;
    static const Dir *down;
    static const Dir *down_right;
    static const Dir *right;
    static const Dir *up_right;
    static const Dir *up;
    static const Dir *up_left;
    static const Dir *left;

    const Direction id;
    const int dx, dy;
    const float dist;

    void apply(int *x, int *y) const;
    static const Dir *getDir(int sx, int sy, int ex, int ey);

private:
    Dir(Direction, int, int, float);
};

#endif /* UTILS_DIRECTION_H */
