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

QList<Direction> AStar::path(quint16 x, quint16 y, quint8 z, quint16 ex, quint16 ey, quint8 ez) {
    AStarNode* found = NULL;
    AStarNode* start = new AStarNode(this, NULL, x, y, z, 0, heuristic_->calculate(x, y, z, ex, ey, ez));
    open_.enqueue(start);

    while (!open_.empty()) {
        AStarNode* node = open_.dequeue();

        if (node->x() == ex && node->y() == ey && node->z() == ez) {
            found = node;
            break;
        }

        // Iterate all neighbours
        grid_->forEachNeighbour(node->x(), node->y(), node->z(), [this, ex, ey, ez, node](quint16 nx, quint16 ny, quint16 nz, quint8 cost) {
            const quint32 g = node->g() + cost;
            const quint32 h = heuristic_->calculate(nx, ny, nz, ex, ey, ez) * 55;

            AStarNode* neighbour = nodeAt(nx, ny, nz);
            if (neighbour != NULL) {
                // This node already exists
                if (neighbour->g() <= g) {
                    // But it is cheaper than us
                    return;
                }
                neighbour->update(node, g, h);
            }
            else {
                // Create a new node
                neighbour = new AStarNode(this, node, nx, ny, nz, g, h);
                open_.enqueue(neighbour);
            }
        });
    }

    QList<Direction> directions;
    if (found == NULL) {
        return directions;
    }

    quint16 px = ex;
    quint16 py = ey;
    quint16 pz = ez;
    while (found != NULL) {
        int dx = px - found->x();
        int dy = py - found->y();

        // TODO change this back to directions
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
        pz = found->z();
        found = found->parent();
    }

    return directions;
}

AStarNode* AStar::nodeAt(quint16 x, quint16 y, quint8 z) const {
    foreach (AStarNode* node, nodes_) {
        if (node->x() == x && node->y() == y && node->z() == z) {
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
    z_(0),
    g_(0),
    h_(0) {
    astar_->nodes_.append(this);
}

AStarNode::AStarNode(AStar *astar, AStarNode* parent, quint16 x, quint16 y, quint8 z, quint32 g, quint32 h):
    astar_(astar),
    parent_(parent),
    x_(x),
    y_(y),
    z_(z),
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
