#include "HpaStarCluster.h"
#include "HpaStarNode.h"
#include "HpaStarEdge.h"
#include "HpaStar.h"

#include "MiniMapView.h"

#include <QDebug>

HpaStarCluster::HpaStarCluster(HpaStar* hpa, quint16 x_, quint16 y_, quint8 z_):
    x(x_),
    y(y_),
    z(z_),
    hpa_(hpa) {
    hpa_->clusters_.append(this);
}

HpaStarCluster::~HpaStarCluster() {
    hpa_->clusters_.removeAll(this);
}

void HpaStarCluster::initializeInterEdges() {
    HpaStarGridInterface* grid = hpa_->grid();

    // Check east inter-borders
    quint16 right = x + CLUSTER_WIDTH - 1;
    HpaStarCluster* rightCluster = hpa_->clusterAt(right + 1, y, z);
    if (rightCluster != NULL) {
        quint8 lastFreeOffset = 0;
        bool lastBlocking = true;
        for (quint8 yOffset = 0; yOffset < CLUSTER_HEIGHT; ++yOffset) {
            // Check if neighbour tiles are both accessible
            bool blocking = grid->blocking(right, y + yOffset, z)|| grid->blocking(right + 1, y + yOffset, z);
            bool last = yOffset == (CLUSTER_HEIGHT - 1);

            if (blocking || last) {
                if (lastFreeOffset != yOffset && !lastBlocking) {
                    // Take the center offset
                    quint8 centerOffset = (yOffset - lastFreeOffset) / 2;

                    HpaStarNode* thisNode = new HpaStarNode(this, right, y + centerOffset, z);
                    HpaStarNode* rightNode = new HpaStarNode(rightCluster, right + 1, y + centerOffset, z);
                    HpaStarEdge* edge = new HpaStarEdge(hpa_, thisNode, rightNode, 1);

                    // Add this edges
                    this->edges_.append(edge);
                    rightCluster->edges_.append(edge);
                }
                // Set new startOffset
                lastFreeOffset = yOffset + 1;
            }
            lastBlocking = blocking;
        }
    }

    // Check west inter-borders
    quint16 bottom = y + CLUSTER_HEIGHT - 1;
    HpaStarCluster* bottomCluster = hpa_->clusterAt(x, bottom + 1, z);
    if (bottomCluster != NULL) {
        quint8 lastFreeOffset = 0;
        bool lastBlocking = true;
        for (quint8 xOffset = 0; xOffset < CLUSTER_WIDTH; ++xOffset) {
            // Check if neighbour tiles are both accessible
            bool blocking = grid->blocking(x + xOffset, bottom, z) || grid->blocking(x + xOffset, bottom + 1, z);
            bool last = xOffset == (CLUSTER_WIDTH - 1);

            if (blocking || last) {
                if (lastFreeOffset != xOffset && !lastBlocking) {
                    // Take the center offset
                    quint8 centerOffset = (xOffset - lastFreeOffset) / 2;

                    HpaStarNode* thisNode = new HpaStarNode(this, x + centerOffset, bottom, z);
                    HpaStarNode* bottomNode = new HpaStarNode(bottomCluster, x + centerOffset, bottom + 1, z);
                    HpaStarEdge* edge = new HpaStarEdge(hpa_, thisNode, bottomNode, 1);

                    // Add this edge
                    this->edges_.append(edge);
                    bottomCluster->edges_.append(edge);
                }
                // Set new startOffset
                lastFreeOffset = xOffset + 1;
            }
            lastBlocking = blocking;
        }
    }

    // Check stairs that go down
    HpaStarCluster* downCluster = hpa_->clusterAt(x, y, z + 1);
    if (downCluster != NULL) {
        for (int xOffset = 0; xOffset < CLUSTER_WIDTH; ++xOffset) {
            for (int yOffset = 0; yOffset < CLUSTER_HEIGHT; ++yOffset) {
                bool floorChange = grid->floorChange(x + xOffset, y + yOffset, z) && grid->floorChange(x + xOffset, y + yOffset, z);
                if (floorChange) {
                    HpaStarNode* thisNode = new HpaStarNode(this, x + xOffset, y + yOffset, z);
                    HpaStarNode* downNode = new HpaStarNode(downCluster, x + xOffset, y + yOffset, z + 1);
                    HpaStarEdge* edge = new HpaStarEdge(hpa_, thisNode, downNode, 1);

                    // Add this edge
                    this->edges_.append(edge);
                    downCluster->edges_.append(edge);
                }
            }
        }
    }
}

void HpaStarCluster::initializeIntraEdges() {
    qDebug() << "initializeIntraEdges" << x << y << z;
    foreach (HpaStarNode* first, nodes_) {
        initializeIntraEdges(first);
    }
}

void HpaStarCluster::initializeIntraEdges(HpaStarNode* node) {
    QRect bounds = QRect(x, y, CLUSTER_WIDTH, CLUSTER_HEIGHT);

    foreach (HpaStarNode* other, nodes_) {
        if (node == other) {
            continue;
        }

        quint32 cost = 0;
        if (hpa_->grid()->reachable(node->position(), other->position(), bounds, &cost)) {
            HpaStarEdge* edge = new HpaStarEdge(hpa_, node, other, cost);

            // Add this edge
            this->edges_.append(edge);
        }
    }
}
