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

#ifndef PATHFINDERPLUGIN_H
#define PATHFINDERPLUGIN_H

#include <AStarGridInterface.h>
#include <MiniMapPluginInterface.h>
#include <PathFinderPluginInterface.h>
#include <PluginInterface.h>

#include <Direction.h>
#include <Position.h>

#include <QObject>

class PathFinderPlugin: public QObject, public PluginInterface, public PathFinderPluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface PathFinderPluginInterface)

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "be.gillis.pathfinder" FILE "meta.js")
#endif

public:
    void install(HookInterface*, SettingsInterface*) throw(std::exception) {}
    void uninstall() {}

    QList<Direction> findPath(AStarGridInterface* grid, quint16 x, quint16 y, quint16 ex, quint16 ey) const;
};

#endif
