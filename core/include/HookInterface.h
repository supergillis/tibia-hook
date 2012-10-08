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

#ifndef HOOKINTERFACE_H
#define HOOKINTERFACE_H

#include <QtGlobal>

class SettingsInterface;
class SenderInterface;
class PluginManagerInterface;
class ProxyInterface;
class ReceiverInterface;
class ReadOnlyProxyInterface;
class UIManagerInterface;
class MemoryInterface;
class HookInterface {
public:
	virtual ~HookInterface() {}

	virtual SettingsInterface* settings() = 0;
	virtual SenderInterface* sender() = 0;
    virtual ReceiverInterface* receiver() = 0;
    virtual PluginManagerInterface* plugins() = 0;
    virtual UIManagerInterface* ui() = 0;
    virtual MemoryInterface* memory() = 0;

    virtual void addOutgoingProxy(quint8, ProxyInterface*) = 0;
    virtual void removeOutgoingProxy(quint8, ProxyInterface*) = 0;

    virtual void addOutgoingReadOnlyProxy(quint8, ReadOnlyProxyInterface*) = 0;
    virtual void removeOutgoingReadOnlyProxy(quint8, ReadOnlyProxyInterface*) = 0;

    virtual void addIncomingReadOnlyProxy(quint8, ReadOnlyProxyInterface*) = 0;
    virtual void removeIncomingReadOnlyProxy(quint8, ReadOnlyProxyInterface*) = 0;
};

#endif
