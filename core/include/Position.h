#ifndef POSITION_H
#define POSITION_H

#include <QtGlobal>
#include <QMetaType>

struct Position {
    quint16 x;
    quint16 y;
    quint16 z;
};

Q_DECLARE_METATYPE(Position)

#endif
