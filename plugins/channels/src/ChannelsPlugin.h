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

#ifndef CHANNELSPLUGIN_H
#define CHANNELSPLUGIN_H

#include <ChannelsPluginInterface.h>
#include <HookInterface.h>
#include <PluginInterface.h>
#include <ProxyInterface.h>

#include <QtPlugin>
#include <QObject>

class ChannelListInjector;
class ChannelsPlugin: public QObject, public PluginInterface, public ChannelsPluginInterface {
	Q_OBJECT
    Q_INTERFACES(PluginInterface ChannelsPluginInterface)

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "be.gillis.channels" FILE "meta.js")
#endif

public:
    void install(HookInterface*, SettingsInterface*) throw(std::exception);
    void uninstall();

    quint16 openChannel(const QString& name);
    void closeChannel(quint16 channelId);

    void postMessage(quint16 channelId, const QString& message);
    void postMessage(quint16 channelId, const QString& message, const QString& name);

    void openPrivateChannel(const QString& name);
    void postPrivateMessage(const QString& name, const QString& message);

private:
    SenderInterface* sender_;
    ChannelListInjector* channels_;
};

#endif
