//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// the map of OpenTibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef ASTARNODES_H
#define ASTARNODES_H

#include <PathFinderPluginInterface.h>
#include <Position.h>

#include <bitset>

#include <QtGlobal>

#define MAX_NODES 512
#define GET_NODE_INDEX(a) (a - &nodes[0])

#define MAP_NORMALWALKCOST 10
#define MAP_DIAGONALWALKCOST 25

struct AStarNode {
    AStarNode* parent;

    quint16 x;
    quint16 y;

    quint32 f;
    quint32 g;
    quint32 h;
};

class AStarNodes {
public:
    AStarNodes();
    ~AStarNodes() {};

    AStarNode* createOpenNode();
    AStarNode* getBestNode();
    void closeNode(AStarNode* node);
    void openNode(AStarNode* node);
    quint32 countClosedNodes();
    quint32 countOpenNodes();
    bool isInList(qint32 x, qint32 y);
    AStarNode* getNodeInList(qint32 x, qint32 y);

    qint32 getMapWalkCost(AStarNode* node, const Position& neighbourPos);
    qint32 getEstimatedDistance(qint32 x, qint32 y, qint32 xGoal, qint32 yGoal);

    static qint32 getTileWalkCost();

private:
    AStarNode nodes[MAX_NODES];
    std::bitset<MAX_NODES> openNodes;
    quint32 curNode;
};

#endif
