#include "HpaStar.h"
#include "HpaStarCluster.h"
#include "HpaStarEdge.h"
#include "HpaStarNode.h"

#include <QDebug>

class Partitioner: public HpaStarPartitioner {
public:
    Partitioner(HpaStar* hpaStar): hpaStar_(hpaStar) {}

    void partition(quint16 x, quint16 y, quint8 z) {
        new HpaStarCluster(hpaStar_, x, y, z);
    }

private:
    HpaStar* hpaStar_;
};

HpaStar::HpaStar(HpaStarGridInterface* grid):
    grid_(grid),
    tempStart_(NULL),
    tempEnd_(NULL) {

    // Let the grid create the partitions
    Partitioner partitioner(this);
    grid_->partition(partitioner);

    foreach (HpaStarCluster* cluster, clusters_) {
        cluster->initializeInterEdges();
        cluster->initializeIntraEdges();
    }
}

HpaStar::~HpaStar() {
    foreach (HpaStarCluster* cluster, clusters_) {
        foreach (HpaStarEdge* edge, cluster->edges_) {
            delete edge;
        }
        foreach (HpaStarNode* node, cluster->nodes_) {
            delete node;
        }
        delete cluster;
    }
}

void HpaStar::forEachNeighbour(const Position& position, AStarVisitor& visitor) {
    HpaStarCluster* cluster = clusterAt(position);
    if (cluster == NULL) {
        return;
    }

    foreach (HpaStarEdge* edge, cluster->edges_) {
        const HpaStarNode* node = NULL;
        if (edge->first->position() == position) {
            node = edge->second;
        }
        else if (edge->second->position() == position) {
            node = edge->first;
        }

        // There is a node on this position
        if (node != NULL) {
            visitor.visitNeighbour(node->position(), edge->cost);
        }
    }
}

void HpaStar::addTemporaryPositions(const Position& start, const Position& end) {
    if (tempStart_ != NULL || tempEnd_ != NULL) {
        return;
    }

    // Add start and end nodes
    HpaStarCluster* startCluster = clusterAt(start);
    HpaStarCluster* endCluster = clusterAt(end);
    if (startCluster == NULL || endCluster == NULL) {
        qDebug() << "startCluster" << startCluster << "endCluster" << endCluster;
        return;
    }

    tempStart_ = new HpaStarNode(startCluster, start);
    tempEnd_ = new HpaStarNode(endCluster, end);

    startCluster->initializeIntraEdges(tempStart_);
    endCluster->initializeIntraEdges(tempEnd_);
}

void HpaStar::removeTemporaryPositions() {
    if (tempStart_ == NULL || tempEnd_ == NULL) {
        return;
    }

    // Remove start edges
    QMutableListIterator<HpaStarEdge*> startIterator(tempStart_->cluster()->edges());
    while (startIterator.hasNext()) {
        HpaStarEdge* edge = startIterator.next();
        if (edge->first == tempStart_ || edge->second == tempStart_) {
            startIterator.remove();
            delete edge;
        }
    }

    // Remove end edges
    QMutableListIterator<HpaStarEdge*> endIterator(tempEnd_->cluster()->edges());
    while (endIterator.hasNext()) {
        HpaStarEdge* edge = endIterator.next();
        if (edge->first == tempEnd_ || edge->second == tempEnd_) {
            endIterator.remove();
            delete edge;
        }
    }

    delete tempStart_;
    delete tempEnd_;
}

HpaStarCluster* HpaStar::clusterAt(const Position& position) const {
    return clusterAt(position.x, position.y, position.z);
}

HpaStarCluster* HpaStar::clusterAt(quint16 x, quint16 y, quint8 z) const {
    quint32 clusterWidth = grid_->clusterWidth();
    quint32 clusterHeight = grid_->clusterHeight();
    foreach (HpaStarCluster* cluster, clusters_) {
        if (cluster->z == z && cluster->x <= x && (cluster->x + clusterWidth) > x && cluster->y <= y && (cluster->y + clusterHeight) > y) {
            return cluster;
        }
    }
    return NULL;
}
