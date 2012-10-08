#ifndef HPASTAR_H
#define HPASTAR_H

#include <AStarGridInterface.h>
#include <HpaStarGridInterface.h>

class HpaStarNode;
class HpaStarEdge;
class HpaStarCluster;
class HpaStar: public AStarGridInterface {
public:
    HpaStar(HpaStarGridInterface* grid);
	virtual ~HpaStar();

    void forEachNeighbour(const Position& position, AStarVisitor& visitor);

    void addTemporaryPositions(const Position& start, const Position& end);
    void removeTemporaryPositions();

    HpaStarCluster* clusterAt(const Position& position) const;
    HpaStarCluster* clusterAt(quint16 x, quint16 y, quint8 z) const;

    HpaStarGridInterface* grid() { return grid_; }

protected:
    QList<HpaStarCluster*> clusters_;
    QList<HpaStarEdge*> edges_;

    friend class HpaStarCluster;
    friend class HpaStarEdge;

private:
    HpaStarGridInterface* grid_;

    HpaStarNode* tempStart_;
    HpaStarNode* tempEnd_;
};

#endif
