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

#ifndef TEMPLATEABLEPROXYMANAGER_H
#define TEMPLATEABLEPROXYMANAGER_H

#include <stdexcept>

#include <QHash>
#include <QList>

#include <PacketReader.h>
#include <ProxyInterface.h>
#include <ReadOnlyProxyInterface.h>

template<class ProxyType>
class TemplateableProxyManagerBase {
public:
    inline void append(quint8 type, ProxyType* proxy) {
        this->proxies_[type].append(proxy);
    }

    inline void remove(quint8 type, ProxyType* proxy) {
        this->proxies_[type].removeAll(proxy);
    }

protected:
    QList<ProxyType*> proxies_[256];
};

template<class ProxyType, class HandleType>
class TemplateableProxyManager: public TemplateableProxyManagerBase<ProxyType> {
public:
    HandleType handlePacket(PacketReader& reader);
};

template<class ProxyType>
class TemplateableProxyManager<ProxyType, void>: public TemplateableProxyManagerBase<ProxyType> {
public:
    void handlePacket(PacketReader& reader) {
        quint8 type = reader.peekU8();

        // Iterate all proxies
        QList<ProxyType*> proxies = this->proxies_[type];
        foreach (ProxyType* proxy, proxies) {
            proxy->handlePacket(reader);
        }
    }
};

template<class ProxyType>
class TemplateableProxyManager<ProxyType, bool>: public TemplateableProxyManagerBase<ProxyType> {
public:
    bool handlePacket(PacketReader& reader) {
        quint8 type = reader.peekU8();

        // Iterate all proxies and stop when one returns false
        QList<ProxyType*> proxies = this->proxies_[type];
        foreach (ProxyType* proxy, proxies) {
            if (!proxy->handlePacket(reader)) {
                return false;
            }
        }
        return true;
    }
};

typedef TemplateableProxyManager<ProxyInterface, bool> ProxyManager;
typedef TemplateableProxyManager<ReadOnlyProxyInterface, void> ReadOnlyProxyManager;

#endif
