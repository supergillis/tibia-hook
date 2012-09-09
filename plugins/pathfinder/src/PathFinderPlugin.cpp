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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.pathfinder, PathFinderPlugin)
#endif

void PathFinderPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
}

void PathFinderPlugin::uninstall() {
}

QList<Position> PathFinderPlugin::findPath(MiniMapPluginInterface* map, const Position& start, const Position& end) const {
    MiniMapFloorInterface* floor = map->floor(start.z);

    return JumpPointSearch(floor, start.x, start.y, end.x, end.y).positions();
}
