#ifndef HPASTAREDGE_H
#define HPASTAREDGE_H

#include <QtGlobal>

class HpaStarNode;
class HpaStar;
class HpaStarEdge {
public:
    HpaStarEdge(HpaStar* grid, HpaStarNode* first_, HpaStarNode* second_, quint32 cost_);
    ~HpaStarEdge();

    const HpaStarNode* first;
    const HpaStarNode* second;
    const quint32 cost;

private:
    HpaStar* hpa_;
};

#endif
