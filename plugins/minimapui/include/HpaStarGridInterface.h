#ifndef HPASTARGRIDINTERFACE_H
#define HPASTARGRIDINTERFACE_H

#include <QRect>

#include <Position.h>

class HpaStarPartitioner {
public:
    virtual void partition(quint16 x, quint16 y, quint8 z) = 0;
};

class HpaStarGridInterface {
public:
    virtual quint32 clusterWidth() const = 0;
    virtual quint32 clusterHeight() const  = 0;

    virtual bool blocking(const Position& position) const = 0;
    virtual bool blocking(quint16 x, quint16 y, quint8 z) const = 0;

    virtual bool floorChange(const Position& position) const = 0;
    virtual bool floorChange(quint16 x, quint16 y, quint8 z) const = 0;

    virtual void partition(HpaStarPartitioner& partitioner) = 0;
    virtual bool reachable(const Position& start, const Position& end, const QRect& boundary, quint32* cost = NULL) const = 0;
};

#endif
