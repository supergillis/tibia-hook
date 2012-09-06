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

#ifndef SCRIPTHOOK_H
#define SCRIPTHOOK_H

#include <QStringList>

#include <HookInterface.h>
#include <PluginInterface.h>
#include <PluginManagerInterface.h>
#include <PacketBuilderInterface.h>
#include <ReceiverInterface.h>
#include <SenderInterface.h>
#include <SettingsInterface.h>

#include "DetourManager.h"
#include "PluginManager.h"
#include "UIManager.h"
#include "Settings.h"

class Hook: public QObject, public HookInterface, public ReceiverInterface {
	Q_OBJECT

public:
    Hook(SettingsInterface*, SenderInterface*, QObject* = 0);

    UIManagerInterface* ui() { return &ui_; }
    PluginManagerInterface* plugins() { return &plugins_; }
    SettingsInterface* settings() { return settings_; }
    SenderInterface* sender() { return sender_; }
    ReceiverInterface* receiver() { return this; }

    PacketBuilderInterface* buildPacket() const;
    PacketBuilderInterface* buildPacket(const PacketInterface*) const;
    PacketBuilderInterface* buildPacket(const QByteArray&) const;
    PacketBuilderInterface* buildPacket(const quint8*, quint16) const;

	bool receiveOutgoingMessage(const QByteArray&);
	void receiveIncomingMessage(const QByteArray&);

    QObject* findPluginByName(const QString& name);

private:
    UIManager ui_;
    PluginManager plugins_;

    SettingsInterface* settings_;
    SenderInterface* sender_;
};

#endif
