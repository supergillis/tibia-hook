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

#include <AStarGridInterface.h>
#include <AStarHeuristicInterface.h>

#include <Direction.h>

#include <QtGlobal>
#include <QList>

class AStarDefaultHeuristic: public AStarHeuristicInterface {
public:
    quint32 calculate(quint16 x, quint16 y, quint8 z, quint16 ex, quint16 ey, quint8 ez) {
        return qAbs(x - ex) + qAbs(y - ey) + qAbs(z - ez) * 50;
    }
};

class AStar;
class AStarNode {
public:
    AStarNode(AStar* astar);
    AStarNode(AStar* astar, AStarNode* parent, quint16 x, quint16 y, quint8 z, quint32 g, quint32 h);
    ~AStarNode();

    void update(AStarNode* parent, quint32 g, quint32 h);

    AStarNode* parent() { return parent_; }

    quint16 x() const { return x_; }
    quint16 y() const { return y_; }
    quint16 z() const { return z_; }

    quint32 g() const { return g_; }
    quint32 h() const { return h_; }
    quint32 f() const { return g_ + h_; }

private:
    AStar* astar_;
    AStarNode* parent_;

    quint16 x_;
    quint16 y_;
    quint8 z_;

    quint32 g_;
    quint32 h_;
};

class AStarNodeComparator {
public:
    bool operator() (const AStarNode* a, const AStarNode* b) const {
        return a->f() < b->f();
    }
};

class AStar {
public:
    AStar(AStarGridInterface* grid, AStarHeuristicInterface* heuristic);
    ~AStar();

    QList<Direction> path(quint16 x, quint16 y, quint8 z, quint16 ex, quint16 ey, quint8 ez);

protected:
    QList<AStarNode*> nodes_;

    friend class AStarNode;

private:
    AStarNode* nodeAt(quint16 x, quint16 y, quint8 z) const;

    AStarGridInterface* grid_;
    AStarHeuristicInterface* heuristic_;

    PriorityQueue<AStarNode*, AStarNodeComparator> open_;
};

#endif
