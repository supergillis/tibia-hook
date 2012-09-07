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

#include <QList>

#include <HookInterface.h>
#include <PacketBuilderInterface.h>
#include <PluginInterface.h>
#include <PluginManagerInterface.h>
#include <ProxyInterface.h>
#include <ReceiverInterface.h>
#include <SenderInterface.h>
#include <SettingsInterface.h>

#include "DetourManager.h"
#include "PluginManager.h"
#include "ProxyManager.h"
#include "UIManager.h"
#include "Settings.h"

typedef QList<ProxyInterface*> ProxyInterfaceList;
typedef QList<ReadOnlyProxyInterface*> ReadOnlyProxyInterfaceList;

class ClientBufferHandler;
class ServerBufferHandler;

class Hook: public HookInterface, public ReceiverInterface {
public:
    Hook();
    ~Hook();

    UIManagerInterface* ui() { return &ui_; }
    PluginManagerInterface* plugins() { return &plugins_; }
    SettingsInterface* settings() { return settings_; }
    SenderInterface* sender() { return sender_; }
    ReceiverInterface* receiver() { return this; }

    void addOutgoingProxy(quint8, ProxyInterface*);
    void removeOutgoingProxy(quint8, ProxyInterface*);

    void addOutgoingReadOnlyProxy(quint8, ReadOnlyProxyInterface*);
    void removeOutgoingReadOnlyProxy(quint8, ReadOnlyProxyInterface*);

    void addIncomingReadOnlyProxy(quint8, ReadOnlyProxyInterface*);
    void removeIncomingReadOnlyProxy(quint8, ReadOnlyProxyInterface*);

	bool receiveOutgoingMessage(const QByteArray&);
    void receiveIncomingMessage(const QByteArray&);

private:
    PluginManager plugins_;
    UIManager ui_;

    ClientBufferHandler* clientBufferHandler_;
    ServerBufferHandler* serverBufferHandler_;

    ProxyManager outgoingProxies_;
    ReadOnlyProxyManager outgoingReadOnlyProxies_;
    ReadOnlyProxyManager incomingProxies_;

    SettingsInterface* settings_;
    SenderInterface* sender_;
};

class ClientBufferHandler: public BufferHandler {
public:
    ClientBufferHandler(SenderInterface* sender, ReceiverInterface* receiver): sender_(sender), receiver_(receiver) {}

    inline void handle(const QByteArray& data) {
        if (receiver_->receiveOutgoingMessage(data)) {
            sender_->sendToServer(data);
        }
    }

private:
    SenderInterface* sender_;
    ReceiverInterface* receiver_;
};

class ServerBufferHandler: public BufferHandler {
public:
    ServerBufferHandler(ReceiverInterface* receiver): receiver_(receiver) {}

    inline void handle(const QByteArray& data) {
        receiver_->receiveIncomingMessage(data);
    }

private:
    ReceiverInterface* receiver_;
};

#endif
