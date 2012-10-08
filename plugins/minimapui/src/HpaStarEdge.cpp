#include "HpaStarEdge.h"
#include "HpaStar.h"

HpaStarEdge::HpaStarEdge(HpaStar* hpa, HpaStarNode* first_, HpaStarNode* second_, quint32 cost_):
    first(first_),
    second(second_),
    cost(cost_),
    hpa_(hpa) {
    hpa_->edges_.append(this);
}

HpaStarEdge::~HpaStarEdge() {
    hpa_->edges_.removeAll(this);
}
