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

AStar::AStar(AStarGridInterface* grid, AStarHeuristicInterface* heuristic):
    grid_(grid),
    heuristic_(heuristic) {
}

AStar::~AStar() {
    foreach (AStarNode* node, nodes_) {
        delete node;
    }
}

QList<Direction> AStar::path(quint16 x, quint16 y, quint16 ex, quint16 ey) {
    static qint8 dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
    static qint8 dy[8] = {0, 0, 1, -1, 1, 1, -1, -1};

    AStarNode* found = NULL;
    AStarNode* start = new AStarNode(this, NULL, x, y, 0, heuristic_->calculate(x, y, ex, ey));
    open_.enqueue(start);

    while (!open_.empty()) {
        AStarNode* node = open_.dequeue();

        if (node->x() == ex && node->y() == ey) {
            found = node;
            break;
        }

        // Iterate all neighbours
        for (quint8 index = 0; index < 4; ++index) {
            const quint16 nx = node->x() + dx[index];
            const quint16 ny = node->y() + dy[index];

            if (grid_->blocking(nx, ny)) {
                continue;
            }

            const quint32 cost = (grid_->cost(nx, ny) * 100) / grid_->averageCost();
            const quint32 g = node->g() + cost;
            const quint32 h = heuristic_->calculate(nx, ny, ex, ey) * 55;

            AStarNode* neighbour = nodeAt(nx, ny);
            if (neighbour != NULL) {
                // This node already exists
                if (neighbour->g() <= g) {
                    // But it is cheaper than us
                    continue;
                }
                neighbour->update(node, g, h);
            }
            else {
                // Create a new node
                neighbour = new AStarNode(this, node, nx, ny, g, h);
                open_.enqueue(neighbour);
            }
        }
    }

    QList<Direction> directions;
    if (found == NULL) {
        return directions;
    }

    quint16 px = ex;
    quint16 py = ey;
    while (found != NULL) {
        int dx = px - found->x();
        int dy = py - found->y();

        if (dx == -1 && dy == -1) {
            directions.prepend(NORTHWEST);
        }
        else if (dx == 1 && dy == -1) {
            directions.prepend(NORTHEAST);
        }
        else if (dx == -1 && dy == 1) {
            directions.prepend(SOUTHWEST);
        }
        else if (dx == 1 && dy == 1) {
            directions.prepend(SOUTHEAST);
        }
        else if (dx == -1) {
            directions.prepend(WEST);
        }
        else if (dx == 1) {
            directions.prepend(EAST);
        }
        else if (dy == -1) {
            directions.prepend(NORTH);
        }
        else if (dy == 1) {
            directions.prepend(SOUTH);
        }

        px = found->x();
        py = found->y();
        found = found->parent();
    }

    return directions;
}

AStarNode* AStar::nodeAt(quint16 x, quint16 y) const {
    foreach (AStarNode* node, nodes_) {
        if (node->x() == x && node->y() == y) {
            return node;
        }
    }
    return NULL;
}

AStarNode::AStarNode(AStar* astar):
    astar_(astar),
    parent_(NULL),
    x_(0),
    y_(0),
    g_(0),
    h_(0) {
    astar_->nodes_.append(this);
}

AStarNode::AStarNode(AStar *astar, AStarNode* parent, quint16 x, quint16 y, quint32 g, quint32 h):
    astar_(astar),
    parent_(parent),
    x_(x),
    y_(y),
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
