#ifndef ASTARVISITOR_H
#define ASTARVISITOR_H

#include <QtGlobal>

#include <Position.h>

class AStarVisitor {
public:
    virtual void visitNeighbour(const Position& position, quint8 cost) = 0;
};

#endif
