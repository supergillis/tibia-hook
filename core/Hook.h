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
#include "Settings.h"

class Hook: public QObject, public HookInterface, public ReceiverInterface, public PluginManagerInterface {
	Q_OBJECT

public:
	Hook(SettingsInterface*, SenderInterface*, QObject* = 0);
	~Hook();

	SettingsInterface* settings() { return settings_; }
    SenderInterface* sender() { return sender_; }
    ReceiverInterface* receiver() { return this; }
    PluginManagerInterface* plugins() { return this; }

    PacketBuilderInterface* createPacketBuilder() const;
    PacketBuilderInterface* createPacketBuilder(const PacketInterface*) const;
    PacketBuilderInterface* createPacketBuilder(const QByteArray&) const;
    PacketBuilderInterface* createPacketBuilder(const quint8*, quint16) const;

	bool receiveFromClient(const QByteArray&);
	void receiveFromServer(const QByteArray&);

    PluginInterface* findPluginByName(const QString& name);

private:
	SettingsInterface* settings_;
	SenderInterface* sender_;

	QList<PluginInterface*> plugins_;
};

#endif
