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
#include "DetourManager.h"
#include "DetourSender.h"
#include "Packet.h"
#include "PacketReader.h"
#include "PacketBuilder.h"
#include "PluginManager.h"
#include "Settings.h"
#include "UIManager.h"

#include <stdexcept>

#define SETTINGS_FILE "config.js"
#define SETTING_PLUGINS_DIRECTORY "plugins"

Hook::Hook():
    plugins_(this),
    ui_() {
    // Try to load the configuration file. Throw an exception on failure
    QFile configFile(SETTINGS_FILE);
    if (!configFile.open(QFile::ReadOnly)) {
        throw std::runtime_error("Could not open config.js!");
    }

    settings_ = new Settings(configFile.readAll());
    if (!settings_->contains(SETTING_PLUGINS_DIRECTORY)) {
        throw std::runtime_error("Could not load plugins directory!");
    }

    sender_ = new DetourSender(DetourManager::instance());
    clientBufferHandler_ = new ClientBufferHandler(sender_, this);
    serverBufferHandler_ = new ServerBufferHandler(this);

    // Connect the DetourManager with the sender and receiver
    DetourManager::instance()->setClientBufferHandler(clientBufferHandler_);
    DetourManager::instance()->setServerBufferHandler(serverBufferHandler_);

    // Load plugins from the given plugins directory
    QString pluginsDir = settings_->value(SETTING_PLUGINS_DIRECTORY).toString();
    plugins_.load(pluginsDir);
}

Hook::~Hook() {
    // Unload plugins
    plugins_.unload();

    // Delete objects
    delete settings_;
    delete sender_;
    delete clientBufferHandler_;
    delete serverBufferHandler_;
}

void Hook::addOutgoingProxy(quint8 type, ProxyInterface* proxy) {
    outgoingProxies_.append(type, proxy);
}

void Hook::removeOutgoingProxy(quint8 type, ProxyInterface* proxy) {
    outgoingProxies_.remove(type, proxy);
}

void Hook::addOutgoingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    outgoingReadOnlyProxies_.append(type, proxy);
}

void Hook::removeOutgoingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    outgoingReadOnlyProxies_.remove(type, proxy);
}

void Hook::addIncomingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    incomingProxies_.append(type, proxy);
}

void Hook::removeIncomingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    incomingProxies_.remove(type, proxy);
}

bool Hook::receiveOutgoingMessage(const QByteArray& data) {
    Packet packet(data);
    PacketReader reader(packet);

    // First call outgoing read only proxies
    outgoingReadOnlyProxies_.handlePacket(reader);

    // Then call read write proxies
    return outgoingProxies_.handlePacket(reader);
}

void Hook::receiveIncomingMessage(const QByteArray& data) {
    Packet packet(data);
    PacketReader reader(packet);

    return incomingProxies_.handlePacket(reader);
}

PacketBuilderInterface* Hook::createPacket() const {
    return new PacketBuilder();
}

PacketBuilderInterface* Hook::createPacket(quint16 length) const {
    return new PacketBuilder(length);
}
