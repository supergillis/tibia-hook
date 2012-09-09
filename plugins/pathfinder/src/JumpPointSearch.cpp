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

#include "JumpPointSearch.h"

bool operator==(const Position2D& lhs, const Position2D& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

/**
  * Thanks to https://github.com/qiao/PathFinding.js/blob/ed09a87108e498860d24697176852da7c9e409e4/src/finders/jump_point.js
  */
JumpPointSearch::JumpPointSearch(MiniMapFloorInterface* floor, quint16 startX, quint16 startY, quint16 endX, quint16 endY):
    floor_(floor),
    startX_(startX),
    startY_(startY),
    endX_(endX),
    endY_(endY) {
}

JumpPointSearch::~JumpPointSearch() {
    foreach (const JpsNode* node, nodes_) {
        delete node;
    }
}

/**
 * Find and return the the path
 */
QList<Position> JumpPointSearch::checkpoints() {
    JpsNode* found = NULL;
    JpsNode* node = new JpsNode();
    node->x = startX_;
    node->y = startY_;
    node->g = 0;
    node->h = heuristic(startX_, startY_);

    nodes_.append(node);
    open_.enqueue(node);

    while (!open_.empty()) {
        JpsNode* best = open_.dequeue();

        // Don't consider this node as a successor
        closed_.append(Position2D(best->x, best->y));

        // If this is the end then exit the loop
        if (best->x == endX_ && best->y == endY_) {
            found = best;
            break;
        }

        // Add successors
        identifySuccessors(best);
    }

    QList<Position> positions;
    if (found == NULL) {
        return positions;
    }

    Position position;
    position.z = floor_->z();
    while (found != NULL) {
        position.x = found->x;
        position.y = found->y;
        positions.push_front(position);

        found = found->parent;
    }

    return positions;
}

/**
 * Find and return the the path
 */
QList<Position> JumpPointSearch::positions() {
    QList<Position> checks(checkpoints());
    QList<Position> positions;

    // Construct full position list
    quint16 currentX = startX_;
    quint16 currentY = startY_;
    quint8 currentZ = floor_->z();
    while (!checks.empty()) {
        Position checkpoint = checks.first();
        int dx = checkpoint.x - currentX;
        int dy = checkpoint.y - currentY;

        // Directly accessible
        if (dx <= 1 && dx >= -1 && dy <= 1 && dy >= -1) {
            if (dx == 0 && dy == 0) {
                // Same position
            }
            else if (dx != 0 && dy != 0) {
                // Diagonal, convert to two non-diagonal moves
                if (!floor_->blocking(currentX + dx, currentY) && !floor_->blocking(currentX + dx, currentY + dy)) {
                    // It is possible to go horizontal and then vertical
                    positions.append(Position(currentX + dx, currentY, currentZ));
                    positions.append(Position(currentX + dx, currentY + dy, currentZ));
                }
                else if (!floor_->blocking(currentX, currentY + dy) && !floor_->blocking(currentX + dx, currentY + dy)) {
                    // It is possible to go vertical and then horizontal
                    positions.append(Position(currentX, currentY + dy, currentZ));
                    positions.append(Position(currentX + dx, currentY + dy, currentZ));
                }
                else {
                    // It is not possible to go horizontal or vertical, then we need to go diagonal
                    positions.append(checkpoint);
                }
            }
            else {
                positions.append(checkpoint);
            }

            checks.takeFirst();
            currentX = checkpoint.x;
            currentY = checkpoint.y;
        }
        else {
            Position next;
            next.x = dx == 0 ? currentX : dx < 0 ? currentX - 1 : currentX + 1;
            next.y = dy == 0 ? currentY : dy < 0 ? currentY - 1 : currentY + 1;
            next.z = currentZ;
            checks.prepend(next);
        }
    }
    return positions;
}

/**
 * Check if a point on the grid is present and walkable.
 */
bool JumpPointSearch::isOpen(quint16 x, quint16 y) {
    return floor_->boundary().contains(x, y) && !floor_->blocking(x, y);
}

/**
 * Identify successors for the given node. Runs a jump point search in the
 * direction of each available neighbor, adding any points found to the open
 * list.
 */
void JumpPointSearch::identifySuccessors(JpsNode* node) {
    QList<Position2D> neighbors;
    findNeighbours(node, neighbors);

    foreach (Position2D neighbor, neighbors) {
        quint16 nx = neighbor.x;
        quint16 ny = neighbor.y;

        Position2D jumpPoint = jump(nx, ny, node->x, node->y);
        if (jumpPoint.x != 0 && jumpPoint.y != 0) {
            createSuccessor(jumpPoint.x, jumpPoint.y, node);
        }
    }
}

void JumpPointSearch::createSuccessor(quint16 x, quint16 y, JpsNode* parent) {
    if (closed_.contains(Position2D(x, y))) {
        return;
    }

    JpsNode* node = nodeOnPosition(x, y);
    if (node == NULL) {
        node = new JpsNode();
        node->x = x;
        node->y = y;
        nodes_.append(node);
    }
    else {
        // Dequeue and enqueue so that it's position gets updated
        open_.remove(node);
    }

    node->parent = parent;
    node->g = qMax(qAbs(node->x - parent->x), qAbs(node->y - parent->y));
    node->h = heuristic(x, y);
    open_.enqueue(node);
}

quint32 JumpPointSearch::heuristic(quint16 x, quint16 y) {
    return qAbs(x - endX_) + qAbs(y - endY_);
}

JpsNode* JumpPointSearch::nodeOnPosition(quint16 x, quint16 y) {
    QListIterator<JpsNode*> iterator = open_.iterator();
    while (iterator.hasNext()) {
        JpsNode* node = iterator.next();
        if (node->x == x && node->y == y) {
            return node;
        }
    }
    return NULL;
}

/**
 * Search recursively in the direction (parent -> child), stopping only when a
 * jump point is found.
 */
Position2D JumpPointSearch::jump(quint16 x, quint16 y, quint16 px, quint16 py) {
    if (!isOpen(x, y)) {
        return Position2D();
    }
    else if (x == endX_ && y == endY_) {
        return Position2D(x, y);
    }

    int dx = x - px;
    int dy = y - py;

    // check for forced neighbors
    // along the diagonal
    if (dx != 0 && dy != 0) {
        if ((isOpen(x - dx, y + dy) && !isOpen(x - dx, y)) || (isOpen(x + dx, y - dy) && !isOpen(x, y - dy))) {
            return Position2D(x, y);
        }
    }
    // horizontally/vertically
    else {
        if (dx != 0) { // moving along x
            if ((isOpen(x + dx, y + 1) && !isOpen(x, y + 1)) || (isOpen(x + dx, y - 1) && !isOpen(x, y - 1))) {
                return Position2D(x, y);
            }
        }
        else {
            if ((isOpen(x + 1, y + dy) && !isOpen(x + 1, y)) || (isOpen(x - 1, y + dy) && !isOpen(x - 1, y))) {
                return Position2D(x, y);
            }
        }
    }

    // when moving diagonally, must check for vertical/horizontal jump points
    if (dx != 0 && dy != 0) {
        Position2D jx = jump(x + dx, y, x, y);
        Position2D jy = jump(x, y + dy, x, y);
        if ((jx.x != 0 && jx.y != 0) || (jy.x != 0 && jy.y != 0)) {
            return Position2D(x, y);
        }
    }

    // moving diagonally, must make sure one of the vertical/horizontal
    // neighbors is open to allow the path
    if (isOpen(x + dx, y) || isOpen(x, y + dy)) {
        return jump(x + dx, y + dy, x, y);
    }

    return Position2D();
}

/**
 * Find the neighbors for the given node. If the node has a parent,
 * prune the neighbors based on the jump point search algorithm, otherwise
 * return all available neighbors.
 */
void JumpPointSearch::findNeighbours(JpsNode* node, QList<Position2D>& neighbors) {
    quint16 x = node->x;
    quint16 y = node->y;

    // directed pruning: can ignore most neighbors, unless forced.
    if (node->parent) {
        quint16 px = node->parent->x;
        quint16 py = node->parent->y;

        // get the normalized direction of travel
        int dx = (x - px) / qMax(qAbs(x - px), 1);
        int dy = (y - py) / qMax(qAbs(y - py), 1);

        // search diagonally
        if (dx != 0 && dy != 0) {
            if (isOpen(x, y + dy)) {
                neighbors.append(Position2D(x, y + dy));
            }
            if (isOpen(x + dx, y)) {
                neighbors.append(Position2D(x + dx, y));
            }
            if (isOpen(x, y + dy) || isOpen(x + dx, y)) {
                neighbors.append(Position2D(x + dx, y + dy));
            }
            if (!isOpen(x - dx, y) && isOpen(x, y + dy)) {
                neighbors.append(Position2D(x - dx, y + dy));
            }
            if (!isOpen(x, y - dy) && isOpen(x + dx, y)) {
                neighbors.append(Position2D(x + dx, y - dy));
            }
        }
        // search horizontally/vertically
        else {
            if(dx == 0) {
                if (isOpen(x, y + dy)) {
                    if (isOpen(x, y + dy)) {
                        neighbors.append(Position2D(x, y + dy));
                    }
                    if (!isOpen(x + 1, y)) {
                        neighbors.append(Position2D(x + 1, y + dy));
                    }
                    if (!isOpen(x - 1, y)) {
                        neighbors.append(Position2D(x - 1, y + dy));
                    }
                }
            }
            else {
                if (isOpen(x + dx, y)) {
                    if (isOpen(x + dx, y)) {
                        neighbors.append(Position2D(x + dx, y));
                    }
                    if (!isOpen(x, y + 1)) {
                        neighbors.append(Position2D(x + dx, y + 1));
                    }
                    if (!isOpen(x, y - 1)) {
                        neighbors.append(Position2D(x + dx, y - 1));
                    }
                }
            }
        }
    }
    // return all open neighbors
    else {
        static qint8 xOffsets[4] = {-1, 0, 1, 0};
        static qint8 yOffsets[4] = {0, 1, 0, -1};

        static qint8 xDiagonalOffsets[4] = {-1, -1, 1, 1};
        static qint8 yDiagonalOffsets[4] = {-1, 1, 1, -1};

        for (int index = 0; index < 4; ++index) {
            // TODO check if x and y larger than zero
            quint16 nx = x + xOffsets[index];
            quint16 ny = y + yOffsets[index];
            if (isOpen(nx, ny)) {
                neighbors.append(Position2D(nx,ny));
            }

            quint16 dx = xDiagonalOffsets[index];
            quint16 dy = yDiagonalOffsets[index];
            if (isOpen(x + dx, y + dy) && (isOpen(x + dx, y) || isOpen(x, y + dy))) {
                neighbors.append(Position2D(x + dx, y + dy));
            }
        }
    }
}
