#ifndef ASTARNODEINTERFACE_H
#define ASTARNODEINTERFACE_H

#include <QtGlobal>

#include <Position.h>

class AStarNodeInterface {
public:
    virtual AStarNodeInterface* parent() = 0;
    virtual const AStarNodeInterface* parent() const = 0;

    virtual const Position& position() const = 0;

    virtual quint32 g() const = 0;
    virtual quint32 h() const = 0;
    virtual quint32 f() const = 0;
};

#endif
