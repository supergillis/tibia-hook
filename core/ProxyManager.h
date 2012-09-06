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

#include <PacketReaderInterface.h>
#include <ProxyInterface.h>
#include <ReadOnlyProxyInterface.h>

#include <QHash>
#include <QList>

template<typename ProxyType, typename HandleType>
class TemplateableProxyManager {
public:
    inline void append(quint8 type, ProxyType* proxy) {
        proxies_[type].append(proxy);
    }

    inline void remove(quint8 type, ProxyType* proxy) {
        proxies_[type].removeAll(proxy);
    }

    HandleType handlePacket(const PacketReaderInterface* reader);

private:
    typedef QList<ProxyType*> ProxyList;
    typedef QHash<quint8, ProxyList> ProxyListMap;

    ProxyListMap proxies_;
};

typedef TemplateableProxyManager<ProxyInterface, bool> ProxyManager;
typedef TemplateableProxyManager<ReadOnlyProxyInterface, void> ReadOnlyProxyManager;

#endif
