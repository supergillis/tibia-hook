#include <Position.h>
#include <Direction.h>

QDebug operator<< (QDebug debug, const Position& position) {
    debug.nospace() << "[" << position.x << ", " << position.y << ", " << position.z << "]";
    return debug;
}

Position Position::operator+(const Direction& direction) const {
    Position moved(this->x, this->y, this->z);
    switch (direction) {
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

Position Position::operator+(const Position& position) const {
    return Position(this->x + position.x, this->y + position.y, this->z + position.z);
}

bool Position::operator!=(const Position& position) const {
    return this->x != position.x  || this->y != position.y || this->z != position.z;
}

bool Position::operator==(const Position& position) const {
    return this->x == position.x  && this->y == position.y && this->z == position.z;
}
