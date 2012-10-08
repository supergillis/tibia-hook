#ifndef ASTARNODE_H
#define ASTARNODE_H

#include <QtGlobal>

#include <AStarNodeInterface.h>
#include <Position.h>

class AStar;
class AStarNode: public AStarNodeInterface {
public:
    AStarNode(AStar* astar, AStarNode* parent, const Position& position, quint32 g, quint32 h);
    AStarNode(AStar* astar, AStarNode* parent, quint16 x, quint16 y, quint8 z, quint32 g, quint32 h);
    ~AStarNode();

    AStarNode* parent() { return parent_; }
    const AStarNode* parent() const { return parent_; }

    const Position& position() const { return position_; }

    quint32 g() const { return g_; }
    quint32 h() const { return h_; }
    quint32 f() const { return g_ + h_; }

protected:
    void update(AStarNode* parent, quint32 g, quint32 h);

    friend class AStar;
    friend class AStarDefaultVisitor;

private:
    AStar* astar_;
    AStarNode* parent_;

    const Position position_;

    quint32 g_;
    quint32 h_;
};

#endif
