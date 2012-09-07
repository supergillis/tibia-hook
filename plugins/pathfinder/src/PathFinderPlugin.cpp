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

#include "PathFinderPlugin.h"
#include "Path.h"
#include "AStarNodes.h"

#include <MiniMapFloorInterface.h>
#include <PathInterface.h>

#include <stdexcept>

#include <QVariantMap>
#include <QPainter>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.pathfinder, PathFinderPlugin)
#endif

void PathFinderPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
}

void PathFinderPlugin::uninstall() {
}

PathInterface* PathFinderPlugin::createNewPath(MiniMapPluginInterface* map, const Position& start, const Position& end, int maxSearchDist) const {
    QList<Position> positions;

    if (start.z != end.z) {
        return NULL;
    }

    AStarNodes nodes;
    AStarNode* startNode = nodes.createOpenNode();
    startNode->x = start.x;
    startNode->y = start.y;
    startNode->g = 0;
    startNode->h = nodes.getEstimatedDistance(start.x, start.y, end.x, end.y);
    startNode->f = startNode->g + startNode->h;
    startNode->parent = NULL;
    Position pos;
    pos.z = start.z;
    static int32_t neighbourOrderList[8][2] = {
        { -1, 0},
        {0, 1},
        {1, 0},
        {0, -1},

        //diagonal
        { -1, -1},
        {1, -1},
        {1, 1},
        { -1, 1},
    };

    MiniMapFloorInterface* floor = map->floor(start.z);

    AStarNode* found = NULL;

    while (maxSearchDist != -1 || nodes.countClosedNodes() < 100)
    {
        AStarNode* n = nodes.getBestNode();

        if (!n) {
            //no path found
            return NULL;
        }

        if (n->x == end.x && n->y == end.y) {
            found = n;
            break;
        }
        else
        {
            for (int i = 0; i < 8; ++i)
            {
                pos.x = n->x + neighbourOrderList[i][0];
                pos.y = n->y + neighbourOrderList[i][1];
                bool outOfRange = false;

                if (maxSearchDist != -1 && (std::abs(end.x - pos.x) > maxSearchDist ||
                                            std::abs(end.y - pos.y) > maxSearchDist))
                {
                    outOfRange = true;
                }

                if (floor->dataAt(pos.x, pos.y) == 255) {
                    outOfRange = true;
                }

                if (!outOfRange)
                {
                    //The cost (g) for this neighbour
                    int32_t cost = nodes.getMapWalkCost(n, pos);
                    int32_t extraCost = nodes.getTileWalkCost();
                    int32_t newg = n->g + cost + extraCost;
                    //Check if the node is already in the closed/open list
                    //If it exists and the nodes already on them has a lower cost (g) then we can ignore this neighbour node
                    AStarNode* neighbourNode = nodes.getNodeInList(pos.x, pos.y);

                    if (neighbourNode)
                    {
                        if (neighbourNode->g <= newg)
                        {
                            //The node on the closed/open list is cheaper than this one
                            continue;
                        }

                        nodes.openNode(neighbourNode);
                    }
                    else
                    {
                        //Does not exist in the open/closed list, create a new node
                        neighbourNode = nodes.createOpenNode();

                        if (!neighbourNode)
                        {
                            //seems we ran out of nodes
                            return NULL;
                        }
                    }

                    //This node is the best node so far with this state
                    neighbourNode->x = pos.x;
                    neighbourNode->y = pos.y;
                    neighbourNode->parent = n;
                    neighbourNode->g = newg;
                    neighbourNode->h = nodes.getEstimatedDistance(neighbourNode->x, neighbourNode->y,
                                       end.x, end.y);
                    neighbourNode->f = neighbourNode->g + neighbourNode->h;
                }
            }

            nodes.closeNode(n);
        }
    }

    int32_t prevx = end.x;
    int32_t prevy = end.y;
    int32_t dx, dy;

    if (!found) {
        return NULL;
    }

    found = found->parent;

    Position position;
    position.z = start.z;

    while (found)
    {
        /*pos.x = found->x;
        pos.y = found->y;
        dx = pos.x - prevx;
        dy = pos.y - prevy;
        prevx = pos.x;
        prevy = pos.y;

        if (dx == 1 && dy == 1)
        {
            dirList.push_front(NORTHWEST);
        }
        else if (dx == -1 && dy == 1)
        {
            dirList.push_front(NORTHEAST);
        }
        else if (dx == 1 && dy == -1)
        {
            dirList.push_front(SOUTHWEST);
        }
        else if (dx == -1 && dy == -1)
        {
            dirList.push_front(SOUTHEAST);
        }
        else if (dx == 1)
        {
            dirList.push_front(WEST);
        }
        else if (dx == -1)
        {
            dirList.push_front(EAST);
        }
        else if (dy == 1)
        {
            dirList.push_front(NORTH);
        }
        else if (dy == -1)
        {
            dirList.push_front(SOUTH);
        }*/

        position.x = found->x;
        position.y = found->y;
        positions.push_front(position);

        found = found->parent;
    }

    return new Path(positions);
}
