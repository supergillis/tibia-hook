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
#include "AStar.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.pathfinder, PathFinderPlugin)
#endif

bool PathFinderPlugin::findPath(AStarGridInterface& grid, AStarPathBuilderInterface& builder, const Position& start, const Position& end, const QRect& boundary, quint32* cost) const {
    AStarDefaultHeuristic heuristic;
    AStar astar(grid, heuristic);
    astar.setBoundary(boundary);

    return astar.path(builder, start, end, cost);
}
