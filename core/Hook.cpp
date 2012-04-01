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

#include "Hook.h"
#include "Message.h"
#include "ReadOnlyPacketProxy.h"
#include "ReadWritePacketProxy.h"

#include "messages/ServerPlayerStatus.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPluginLoader>

Hook::Hook(SettingsInterface* settings, SenderInterface* sender, QObject* parent) :
	HookInterface(), settings_(settings), sender_(sender) {
	Message::registerServerMessage(new ServerPlayerStatus);
}

Hook::~Hook() {
	foreach(PluginInterface* plugin, plugins_) {
		qDebug() << "uninstalling" << plugin->name();
		plugin->uninstall();
	}
}

ReadOnlyPacketInterface* Hook::createReadOnlyPacket(const QByteArray& data) {
	return new ReadOnlyPacketProxy(data);
}

ReadOnlyPacketInterface* Hook::createReadOnlyPacket(const quint8* data, quint16 length) {
	return new ReadOnlyPacketProxy(data, length);
}

ReadWritePacketInterface* Hook::createReadWritePacket() {
	return new ReadWritePacketProxy();
}

bool Hook::receiveFromClient(const QByteArray& data) {
	ReadOnlyPacket packet(data);
	quint8 type = packet.readU8();
	qDebug() << "client" << type;
	return true;
}

void Hook::receiveFromServer(const QByteArray& data) {
	ReadOnlyPacket packet(data);
	quint8 type = packet.readU8();
	qDebug() << "server" << type;
	Message* message = Message::lookupServerMessage(type);
	if(message != NULL) {
		qDebug() << "message found";
		MessageData* data = message->deserialize(&packet);
		if(type == 0xA0) {
			ServerPlayerStatusData* status = (ServerPlayerStatusData*) data;
			qDebug() << "health" << status->health;
		}
		delete data;
	}
}
