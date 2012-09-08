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
#include "JumpPointSearch.h"

#include <MiniMapFloorInterface.h>

#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.pathfinder, PathFinderPlugin)
#endif

void PathFinderPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
}

void PathFinderPlugin::uninstall() {
}

QList<Position> PathFinderPlugin::findPath(MiniMapPluginInterface* map, const Position& start, const Position& end) const {
    MiniMapFloorInterface* floor = map->floor(start.z);

    JumpPointSearch search(floor, start.x, start.y, end.x, end.y);

    QList<Position> checkpoints = search.find();
    QList<Position> positions;

    foreach (const Position& checkpoint, checkpoints) {
        qDebug() << "checkpoint" << checkpoint.x << checkpoint.y;
    }

    // Construct full position list
    Position current = start;
    while (!checkpoints.empty()) {
        Position checkpoint = checkpoints.first();
        int dx = checkpoint.x - current.x;
        int dy = checkpoint.y - current.y;

        // Directly accessible
        if (dx <= 1 && dx >= -1 && dy <= 1 && dy >= -1) {
            if(dx == 0 && dy == 0) {
                // Same position
            }
            else if (dx != 0 && dy != 0) {
                // Diagonal, convert to two non-diagonal moves
                // TODO check if it is possible to go non-diagonal
                positions.append(Position(current.x + dx, current.y, current.z));
                positions.append(Position(current.x + dx, current.y + dy, current.z));
            }
            else {
                positions.append(checkpoint);
            }

            checkpoints.takeFirst();
            current = checkpoint;
        }
        else {
            Position next;
            next.x = dx == 0 ? current.x : dx < 0 ? current.x - 1 : current.x + 1;
            next.y = dy == 0 ? current.y : dy < 0 ? current.y - 1 : current.y + 1;
            next.z = current.z;
            checkpoints.prepend(next);
        }
    }

    return positions;
}
