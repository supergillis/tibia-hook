#ifndef DIRECTION_H
#define DIRECTION_H

#include <QtGlobal>

enum Direction {
    NORTH = 0b0001,
    EAST  = 0b0010,
    SOUTH = 0b0100,
    WEST  = 0b1000,

    NORTHEAST = NORTH | EAST,
    SOUTHEAST = SOUTH | EAST,
    SOUTHWEST = SOUTH | WEST,
    NORTHWEST = NORTH | WEST
};

#endif
