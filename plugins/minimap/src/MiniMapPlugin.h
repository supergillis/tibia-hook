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

#ifndef MINIMAPPLUGIN_H
#define MINIMAPPLUGIN_H

#include "MiniMapFloor.h"

#include <HookInterface.h>
#include <MiniMapPluginInterface.h>
#include <MiniMapInterface.h>
#include <PluginInterface.h>

#include <QObject>
#include <QDir>

class MiniMap: public MiniMapInterface {
public:
    MiniMap(const QString& directory);

    MiniMapFloorInterface* createNewFloor(quint8 z) const;

private:
    QString directory_;
};

class MiniMapPlugin: public QObject, public PluginInterface, public MiniMapPluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface MiniMapPluginInterface)

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "be.gillis.minimap" FILE "meta.js")
#endif

public:
    void install(HookInterface*, SettingsInterface*) throw(std::exception);
    void uninstall();

    MiniMapInterface* miniMap();

private:
    MiniMapInterface* miniMap_;
};

#endif
