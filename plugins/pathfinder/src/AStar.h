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

#ifndef ASTAR_H
#define ASTAR_H

#include "PriorityQueue.h"
#include "AStarNode.h"

#include <AStarGridInterface.h>
#include <AStarHeuristicInterface.h>
#include <AStarPathBuilderInterface.h>

#include <Position.h>

#include <QtGlobal>
#include <QList>
#include <QRect>

class AStarNodeComparator {
public:
    bool operator() (const AStarNode* a, const AStarNode* b) const {
        return a->f() < b->f();
    }
};

class AStarDefaultHeuristic: public AStarHeuristicInterface {
public:
    quint32 calculate(const Position& start, const Position& end) {
        return qAbs(start.x - end.x) + qAbs(start.y - end.y) + qAbs(start.z - end.z) * 50;
    }
};

class AStar;
class AStarDefaultVisitor: public AStarVisitor {
public:
    AStarDefaultVisitor(AStar* parent, const Position& end): parent_(parent), end_(end) {}

    void visitNeighbour(const Position& position, quint8 cost);

protected:
    void setCurrentNode(AStarNode* node);

    friend class AStar;

private:
    AStar* parent_;
    AStarNode* node_;
    Position end_;
};

class AStar {
public:
    AStar(AStarGridInterface& grid, AStarHeuristicInterface& heuristic);
    ~AStar();

    virtual bool path(AStarPathBuilderInterface& builder, const Position& start, const Position& end, quint32* cost = NULL);

    AStarGridInterface& grid() { return grid_; }
    AStarHeuristicInterface& heuristic() { return heuristic_; }

    const QRect& boundary() { return bounds_; }
    void setBoundary(const QRect& bounds) { bounds_ = bounds; }

    PriorityQueue<AStarNode*, AStarNodeComparator>& openNodes() { return open_; }
    const PriorityQueue<AStarNode*, AStarNodeComparator>& openNodes() const { return open_; }

protected:
    QList<AStarNode*> nodes_;

    AStarNode* nodeAt(const Position& position) const;

    friend class AStarNode;
    friend class AStarDefaultVisitor;

private:
    AStarGridInterface& grid_;
    AStarHeuristicInterface& heuristic_;

    QRect bounds_;

    PriorityQueue<AStarNode*, AStarNodeComparator> open_;
};

#endif
