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

#include <QObject>
#include <QDir>

#include <HookInterface.h>
#include <PluginInterface.h>

#include "MiniMapPluginInterface.h"
#include "MiniMapInterface.h"

class Position {
public:
    Position(): valid_(false) {}
    Position(quint16 x, quint16 y, quint8 z): valid_(true), x_(x), y_(y), z_(z) {}

    bool isValid() {
        return valid_;
    }

    quint16 x() { return x_; }
    quint16 y() { return y_; }
    quint8 z() { return z_; }

private:
    bool valid_;
    quint16 x_;
    quint16 y_;
    quint8 z_;
};

class MiniMap: public MiniMapInterface {
public:
    static const int MINIMAP_FILE_DIMENSION;

    MiniMap(const QString& directory);

    QImage imageForFloor(quint8 z) const;

private:
    static QImage imageForMapFile(const QString& mapFile);
    static QRect mapFilesBoundary(const QStringList& mapFiles);
    static Position positionForMapFile(const QString &fileName);
    static int mapColor(quint8 color);

    QDir directory_;
};

class MiniMapPlugin: public QObject, public PluginInterface, public MiniMapPluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface MiniMapPluginInterface)

public:
    void install(HookInterface*, SettingsInterface*) throw(std::exception);
    void uninstall();

    MiniMapInterface* miniMap();

private:
    MiniMapInterface* miniMap_;
};

#endif
