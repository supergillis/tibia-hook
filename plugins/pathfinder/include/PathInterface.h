#ifndef PATHINTERFACE_H
#define PATHINTERFACE_H

#include <PathFinderPluginInterface.h>
#include <Position.h>

#include <QList>

class PathInterface {
public:
    virtual ~PathInterface() {}

    virtual QList<Position> path() const = 0;
};

#endif
