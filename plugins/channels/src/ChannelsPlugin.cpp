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

#include "ChannelsPlugin.h"

#include <Constants.h>
#include <PacketCodes.h>

#include <QApplication>
#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(be.gillis.channelsplugin, ChannelsPlugin)
#endif

void ChannelsPlugin::install(HookInterface* hook, SettingsInterface* settings) throw(std::exception) {
    sender_ = hook->sender();
}

void ChannelsPlugin::uninstall() {
    sender_ = NULL;
}

quint16 ChannelsPlugin::openChannel(const QString& name) {
    quint16 channelId = 0xA0;
    PacketBuilderInterface* builder = sender_->createPacket();
    builder->writeU8(PacketCodes::In::ChannelOpen);
    builder->writeU16(channelId);
    builder->writeString(name);
    builder->writeU16(0);
    builder->writeU16(0);

    // Send fake packet
    PacketInterface* packet = builder->build();
    sender_->sendToClient(QByteArray((const char*) packet->data(), packet->length()));

    delete packet;
    delete builder;

    return channelId;
}

void ChannelsPlugin::closeChannel(quint16 channelId) {
    PacketBuilderInterface* builder = sender_->createPacket();
    builder->writeU8(PacketCodes::In::ChannelClose);
    builder->writeU16(channelId);

    // Send fake packet
    PacketInterface* packet = builder->build();
    sender_->sendToClient(QByteArray((const char*) packet->data(), packet->length()));

    delete packet;
    delete builder;
}

void ChannelsPlugin::postMessage(quint16 channelId, const QString& message) {
    postMessage(channelId, message, QApplication::applicationName());
}

void ChannelsPlugin::postMessage(quint16 channelId, const QString& message, const QString& name) {
    PacketBuilderInterface* builder = sender_->createPacket();
    builder->writeU8(PacketCodes::In::Speak);
    builder->writeU32(0);
    builder->writeString(name);
    builder->writeU16(0);
    builder->writeU8(Tibia::Message::Channel);
    builder->writeU16(channelId);
    builder->writeString(message);

    // Send fake packet
    PacketInterface* packet = builder->build();
    sender_->sendToClient(QByteArray((const char*) packet->data(), packet->length()));

    delete packet;
    delete builder;
}

void ChannelsPlugin::openPrivateChannel(const QString& name) {
    quint16 channelId = 0xA1;
    PacketBuilderInterface* builder = sender_->createPacket();
    builder->writeU8(PacketCodes::In::ChannelOpenPrivate);
    builder->writeString(name);

    // Send fake packet
    PacketInterface* packet = builder->build();
    sender_->sendToClient(QByteArray((const char*) packet->data(), packet->length()));

    delete packet;
    delete builder;
}

void ChannelsPlugin::postPrivateMessage(const QString& name, const QString& message) {
    PacketBuilderInterface* builder = sender_->createPacket();
    builder->writeU8(PacketCodes::In::Speak);
    builder->writeU32(0);
    builder->writeString(name);
    builder->writeU16(0);
    builder->writeU8(Tibia::Message::PrivateFrom);
    builder->writeString(message);

    // Send fake packet
    PacketInterface* packet = builder->build();
    sender_->sendToClient(QByteArray((const char*) packet->data(), packet->length()));

    delete packet;
    delete builder;
}
