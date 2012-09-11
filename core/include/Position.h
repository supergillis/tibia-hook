#ifndef POSITION_H
#define POSITION_H

#include <Direction.h>

#include <QDebug>
#include <QMetaType>

struct Position {
    Position(): x(0), y(0), z(0) {}
    Position(quint16 x_, quint16 y_, quint8 z_): x(x_), y(y_), z(z_) {}
    Position(const Position& other): x(other.x), y(other.y), z(other.z) {}

    quint16 x;
    quint16 y;
    quint16 z;

    Position operator+(const Direction& direction) const;
    Position operator+(const Position& position) const;

    bool operator!=(const Position& position) const;
    bool operator==(const Position& position) const;
};

QDebug operator<<(QDebug debug, const Position& position);

Q_DECLARE_METATYPE(Position)

#endif
