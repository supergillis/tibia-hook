#ifndef HPASTARCLUSTER_H
#define HPASTARCLUSTER_H

#include <AStarHeuristicInterface.h>
#include <HpaStarGridInterface.h>

#include <QtGlobal>

class ManhattanDistance: public AStarHeuristicInterface {
public:
    quint32 calculate(const Position& start, const Position& end) {
        return qAbs(start.x - end.x) + qAbs(start.y - end.y) + qAbs(start.z - end.z) * 50;
    }
};

class HpaStarNode;
class HpaStarEdge;
class HpaStar;
class HpaStarCluster {
public:
    HpaStarCluster(HpaStar* hpa, quint16 x_, quint16 y_, quint8 z_);
    ~HpaStarCluster();

    const quint16 x;
    const quint16 y;
    const quint8 z;

    inline QList<HpaStarNode*>& nodes() { return nodes_; }
    inline QList<HpaStarEdge*>& edges() { return edges_; }

protected:
    void initializeInterEdges();
    void initializeIntraEdges();
    void initializeIntraEdges(HpaStarNode* node);

    QList<HpaStarNode*> nodes_;
    QList<HpaStarEdge*> edges_;

    friend class HpaStar;
    friend class HpaStarNode;

private:
    HpaStar* hpa_;
};

#endif // HPASTARCLUSTER_H
