#include "AStarNode.h"
#include "AStar.h"

AStarNode::AStarNode(AStar* astar, AStarNode* parent, const Position& position, quint32 g, quint32 h):
    astar_(astar),
    parent_(parent),
    position_(position),
    g_(g),
    h_(h) {
    astar_->nodes_.append(this);
}

AStarNode::AStarNode(AStar* astar, AStarNode* parent, quint16 x, quint16 y, quint8 z, quint32 g, quint32 h):
    astar_(astar),
    parent_(parent),
    position_(x, y, z),
    g_(g),
    h_(h) {
    astar_->nodes_.append(this);
}

AStarNode::~AStarNode() {
    astar_->nodes_.removeAll(this);
}

void AStarNode::update(AStarNode* parent, quint32 g, quint32 h) {
    parent_ = parent;
    g_ = g;
    h_ = h;

    // Requeue
    astar_->open_.remove(this);
    astar_->open_.enqueue(this);
}
