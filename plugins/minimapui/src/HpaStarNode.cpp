#include "HpaStarNode.h"
#include "HpaStarCluster.h"

HpaStarNode::HpaStarNode(HpaStarCluster* cluster, quint16 x, quint16 y, quint8 z):
    position_(x, y, z),
    cluster_(cluster) {
    cluster_->nodes_.append(this);
}

HpaStarNode::HpaStarNode(HpaStarCluster* cluster, const Position& position):
    position_(position),
    cluster_(cluster) {
    cluster_->nodes_.append(this);
}

HpaStarNode::~HpaStarNode() {
    cluster_->nodes_.removeAll(this);
}
