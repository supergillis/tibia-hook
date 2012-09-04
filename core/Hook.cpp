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
#include "Packet.h"
#include "PacketReader.h"
#include "PacketBuilder.h"
#include "PluginManager.h"
#include "UIManager.h"

#include <stdexcept>

#define SETTING_PLUGINS_DIRECTORY "plugins"

Hook::Hook(SettingsInterface* settings, SenderInterface* sender, QObject* parent) :
    QObject(parent),
    settings_(settings),
    sender_(sender) {
    if(!settings_->contains(SETTING_PLUGINS_DIRECTORY)) {
        throw std::runtime_error("Could not load plugins directory!");
    }

    ui_ = new UIManager();

    QString pluginsDir = settings->value(SETTING_PLUGINS_DIRECTORY).toString();
    plugins_ = new PluginManager(this);
    plugins_->load(pluginsDir);
}

Hook::~Hook() {
    delete ui_;
    delete plugins_;
}

bool Hook::receiveFromClient(const QByteArray& data) {
    Packet packet(data);
    PacketReader reader(&packet);
    quint8 type = reader.readU8();
    qDebug() << "client" << type << " length " << data.length();
    return true;
}

void Hook::receiveFromServer(const QByteArray& data) {
    Packet packet(data);
    PacketReader reader(&packet);
    quint8 type = reader.readU8();
    qDebug() << "server" << type << " length " << data.length();
}

PacketBuilderInterface* Hook::buildPacket() const {
    return new PacketBuilder();
}

PacketBuilderInterface* Hook::buildPacket(const PacketInterface* packet) const {
    return new PacketBuilder(packet);
}

PacketBuilderInterface* Hook::buildPacket(const QByteArray& data) const {
    return new PacketBuilder(data);
}

PacketBuilderInterface* Hook::buildPacket(const quint8* data, quint16 length) const {
    return new PacketBuilder(data, length);
}
