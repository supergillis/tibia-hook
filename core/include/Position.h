/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef POSITION_H
#define POSITION_H

#include <Direction.h>

#include <QMetaType>

struct Position {
    Position(): x(0), y(0), z(0) {}
    Position(quint16 x_, quint16 y_, quint8 z_): x(x_), y(y_), z(z_) {}
    Position(const Position& other): x(other.x), y(other.y), z(other.z) {}

    quint16 x;
    quint16 y;
    quint16 z;

    Position operator+(const Position& rhs) const {
        return Position(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
    }

    Position operator+(const Direction& rhs) const {
        Position moved(this->x, this->y, this->z);
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

    bool operator!=(const Position& rhs) const {
        return this->x != rhs.x  || this->y != rhs.y || this->z != rhs.z;
    }

    bool operator==(const Position& rhs) const {
        return this->x == rhs.x  && this->y == rhs.y && this->z == rhs.z;
    }
};

Q_DECLARE_METATYPE(Position)

#endif
