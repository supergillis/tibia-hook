#include <Position.h>
#include <Direction.h>

QDebug operator<<(QDebug debug, const Position& position) {
    debug.nospace() << "[" << position.x << ", " << position.y << ", " << position.z << "]";
    return debug;
}

Position operator+(const Position& lhs, const Position& rhs) {
    return Position(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Position operator+(const Position& lhs, const Direction& rhs) {
    Position moved(lhs.x, lhs.y, lhs.z);
    switch (rhs) {
    case NORTH: moved.y -= 1; break;
    case NORTHEAST: moved.y -= 1; moved.x += 1; break;
    case EAST: moved.x += 1; break;
    case SOUTHEAST: moved.y -= 1; moved.x += 1;  break;
    case SOUTH: moved.y += 1; break;
    case SOUTHWEST: moved.y -= 1; moved.x -= 1;  break;
    case WEST: moved.x -= 1; break;
    case NORTHWEST: moved.y -= 1; moved.x -= 1;  break;
    }
    return moved;
}

bool operator!=(const Position& lhs, const Position& rhs) {
    return lhs.x != rhs.x  || lhs.y != rhs.y || lhs.z != rhs.z;
}

bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.x == rhs.x  && lhs.y == rhs.y && lhs.z == rhs.z;
}
