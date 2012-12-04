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

#include "PacketReader.h"
#include "ProxyInterface.h"
#include "ProxyManager.h"
#include "ReadOnlyProxyInterface.h"

bool ProxyManager::handleOutgoingPacket(PacketReader& reader) const {
    quint8 type = reader.peekU8();

    // Iterate all read-only proxies first
    foreach (ReadOnlyProxyInterface* proxy, outgoingReadOnlyProxies_[type]) {
        proxy->handlePacket(reader);
    }

    // Iterate all proxies and stop when one returns false
    foreach (ProxyInterface* proxy, outgoingProxies_[type]) {
        if (!proxy->handlePacket(reader)) {
            return false;
        }
    }
    return true;
}

bool ProxyManager::handleIncomingPacket(PacketReader& reader) const {
    quint8 type = reader.peekU8();

    // Iterate read-only proxies
    foreach (ReadOnlyProxyInterface* proxy, incomingReadOnlyProxies_[type]) {
        proxy->handlePacket(reader);
    }
    return true;
}

void ProxyManager::addOutgoingProxy(quint8 type, ProxyInterface* proxy) {
    outgoingProxies_[type].append(proxy);
}

void ProxyManager::removeOutgoingProxy(quint8 type, ProxyInterface* proxy) {
    outgoingProxies_[type].removeAll(proxy);
}

void ProxyManager::addOutgoingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    outgoingReadOnlyProxies_[type].append(proxy);
}

void ProxyManager::removeOutgoingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    outgoingReadOnlyProxies_[type].removeAll(proxy);
}

void ProxyManager::addIncomingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    incomingReadOnlyProxies_[type].append(proxy);
}

void ProxyManager::removeIncomingReadOnlyProxy(quint8 type, ReadOnlyProxyInterface* proxy) {
    incomingReadOnlyProxies_[type].removeAll(proxy);
}
