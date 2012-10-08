#ifndef HPASTARNODE_H
#define HPASTARNODE_H

#include <Position.h>

class HpaStarCluster;
class HpaStarNode {
public:
    HpaStarNode(HpaStarCluster* cluster, quint16 x, quint16 y, quint8 z);
    HpaStarNode(HpaStarCluster* cluster, const Position& position);
    ~HpaStarNode();

    inline const Position& position() const { return position_; }
    inline HpaStarCluster* cluster() { return cluster_; }

private:
    const Position position_;
    HpaStarCluster* cluster_;
};

#endif
