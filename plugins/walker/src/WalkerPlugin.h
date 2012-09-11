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

#ifndef WALKERPLUGIN_H
#define WALKERPLUGIN_H

#include <ChannelsPluginInterface.h>
#include <WalkerPluginInterface.h>
#include <HookInterface.h>
#include <PluginInterface.h>
#include <PositionTrackerPluginInterface.h>

#include <Direction.h>
#include <Position.h>

#include <QtPlugin>
#include <QList>
#include <QObject>

class WalkerPlugin: public QObject, public PluginInterface, public WalkerPluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface WalkerPluginInterface)

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "be.gillis.walker" FILE "meta.js")
#endif

public:
    WalkerPlugin();

    void install(HookInterface*, SettingsInterface*) throw(std::exception);
    void uninstall();

    void walk(const QList<Direction>& directions);
    void move(const Direction& direction);

private slots:
    void moved(const Position& position);

private:
    SenderInterface* sender_;
    PositionTrackerPluginInterface* tracker_;

    QList<Direction> directions_;
    Position next_;
    bool walking_;
};

#endif
