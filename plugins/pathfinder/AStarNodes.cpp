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

#include "AStarNodes.h"

#include <QDebug>

AStarNodes::AStarNodes()
{
    curNode = 0;
    openNodes.reset();
}

AStarNode* AStarNodes::createOpenNode()
{
    if (curNode >= MAX_NODES)
    {
        return NULL;
    }

    quint32 ret_node = curNode;
    curNode++;
    openNodes[ret_node] = 1;
    return &nodes[ret_node];
}

AStarNode* AStarNodes::getBestNode()
{
    if (curNode == 0)
    {
        return NULL;
    }

    int best_node_f = 100000;
    quint32 best_node = 0;
    bool found = false;

    for (quint32 i = 0; i < curNode; ++i)
    {
        if (nodes[i].f < best_node_f && openNodes[i] == 1)
        {
            found = true;
            best_node_f = nodes[i].f;
            best_node = i;
        }
    }

    if (found)
    {
        return &nodes[best_node];
    }

    return NULL;
}

void AStarNodes::closeNode(AStarNode* node)
{
    quint32 pos = GET_NODE_INDEX(node);

    if (pos >= MAX_NODES)
    {
        Q_ASSERT(pos >= MAX_NODES);
        qDebug() << "AStarNodes. trying to close node out of range";
        return;
    }

    openNodes[pos] = 0;
}

void AStarNodes::openNode(AStarNode* node)
{
    quint32 pos = GET_NODE_INDEX(node);

    if (pos >= MAX_NODES)
    {
        Q_ASSERT(pos >= MAX_NODES);
        qDebug() << "AStarNodes. trying to open node out of range";
        return;
    }

    openNodes[pos] = 1;
}

quint32 AStarNodes::countClosedNodes()
{
    quint32 counter = 0;

    for (quint32 i = 0; i < curNode; ++i)
    {
        if (openNodes[i] == 0)
        {
            counter++;
        }
    }

    return counter;
}

quint32 AStarNodes::countOpenNodes()
{
    quint32 counter = 0;

    for (quint32 i = 0; i < curNode; ++i)
    {
        if (openNodes[i] == 1)
        {
            counter++;
        }
    }

    return counter;
}

bool AStarNodes::isInList(int32_t x, int32_t y)
{
    for (quint32 i = 0; i < curNode; ++i)
    {
        if (nodes[i].x == x && nodes[i].y == y)
        {
            return true;
        }
    }

    return false;
}

AStarNode* AStarNodes::getNodeInList(int32_t x, int32_t y)
{
    for (quint32 i = 0; i < curNode; ++i)
    {
        if (nodes[i].x == x && nodes[i].y == y)
        {
            return &nodes[i];
        }
    }

    return NULL;
}

int32_t AStarNodes::getMapWalkCost(AStarNode* node, const Position& neighbourPos)
{
    int cost = 0;

    if (std::abs((int)node->x - neighbourPos.x) == std::abs((int)node->y - neighbourPos.y))
    {
        //diagonal movement extra cost
        cost = MAP_DIAGONALWALKCOST;
    }
    else
    {
        cost = MAP_NORMALWALKCOST;
    }

    return cost;
}

int32_t AStarNodes::getTileWalkCost()
{
    int cost = 0;

    return cost;
}

int32_t AStarNodes::getEstimatedDistance(int32_t x, int32_t y, int32_t xGoal, int32_t yGoal)
{
    int32_t h_diagonal = std::min(std::abs(x - xGoal), std::abs(y - yGoal));
    int32_t h_straight = (std::abs(x - xGoal) + std::abs(y - yGoal));
    return MAP_DIAGONALWALKCOST * h_diagonal + MAP_NORMALWALKCOST * (h_straight - 2 * h_diagonal);
}
