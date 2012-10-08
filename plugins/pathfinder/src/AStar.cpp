/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AStar.h"

void AStarDefaultVisitor::visitNeighbour(const Position& current, quint8 cost) {
    if (!parent_->boundary().isNull() && !parent_->boundary().contains(current.x, current.y)) {
        return;
    }

    const quint32 g = node_->g() + cost;
    const quint32 h = parent_->heuristic().calculate(current, end_) * 55;

    AStarNode* neighbour = parent_->nodeAt(current);
    if (neighbour != NULL) {
        // This node already exists
        if (neighbour->g() <= g) {
            // But it is cheaper than us
            return;
        }
        neighbour->update(node_, g, h);
    }
    else {
        // Create a new node
        neighbour = new AStarNode(parent_, node_, current, g, h);
        parent_->openNodes().enqueue(neighbour);
    }
}

void AStarDefaultVisitor::setCurrentNode(AStarNode* node) {
    node_ = node;
}

AStar::AStar(AStarGridInterface& grid, AStarHeuristicInterface& heuristic):
    grid_(grid),
    heuristic_(heuristic) {
}

AStar::~AStar() {
    foreach (AStarNode* node, nodes_) {
        delete node;
    }
}

bool AStar::path(AStarPathBuilderInterface& builder, const Position& start, const Position &end, quint32 *cost) {
    AStarDefaultVisitor visitor(this, end);
    AStarNode* found = NULL;
    AStarNode* startNode = new AStarNode(this, NULL, start, 0, heuristic_.calculate(start, end));
    open_.enqueue(startNode);

    while (!open_.empty()) {
        AStarNode* node = open_.dequeue();

        if (node->position() == end) {
            found = node;
            break;
        }

        // Iterate all neighbours
        visitor.setCurrentNode(node);
        grid_.forEachNeighbour(node->position(), visitor);
    }

    if (found == NULL) {
        return false;
    }

    if (cost != NULL) {
        *cost = found->g();
    }

    builder.rebuildPath(found);
    return true;
}

AStarNode* AStar::nodeAt(const Position& position) const {
    foreach (AStarNode* node, nodes_) {
        if (node->position() == position) {
            return node;
        }
    }
    return NULL;
}
