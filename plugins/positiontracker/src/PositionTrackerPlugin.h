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

#ifndef POSITIONTRACKERPLUGIN_H
#define POSITIONTRACKERPLUGIN_H

#include <HookInterface.h>
#include <PluginInterface.h>
#include <Position.h>
#include <PositionTrackerPluginInterface.h>
#include <ReadOnlyProxyInterface.h>

#include <QtPlugin>
#include <QObject>

class PositionTrackerPlugin: public QObject, public PluginInterface, public PositionTrackerPluginInterface, public ReadOnlyProxyInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface PositionTrackerPluginInterface)

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "be.gillis.positiontracker" FILE "meta.js")
#endif

public:
    void install(HookInterface*, SettingsInterface*) throw(std::exception);
    void uninstall();

    Position position() const;

    void connectPositionChanged(QObject* object, const char* slot, Qt::ConnectionType type = Qt::AutoConnection);
    void disconnectPositionChanged(QObject* object, const char* slot);

    void handlePacket(PacketReader& reader);

signals:
    void positionChanged(const Position& position);

private:
    HookInterface* hook_;

    Position position_;
};

#endif
