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

#ifndef PROXYRECEIVER_H
#define PROXYRECEIVER_H

#include <Packet.h>
#include <PacketReader.h>
#include <ProxyManagerInterface.h>
#include <ReceiverInterface.h>

class ProxyReceiver: public ReceiverInterface {
public:
    ProxyReceiver(const ProxyManagerInterface* proxies):
        proxies_(proxies) {}

    bool receiveOutgoingMessage(const QByteArray& data) {
        Packet packet(data);
        PacketReader reader(packet);

        return proxies_->handleOutgoingPacket(reader);
    }

    void receiveIncomingMessage(const QByteArray& data) {
        Packet packet(data);
        PacketReader reader(packet);

        // Ignore the return value
        proxies_->handleIncomingPacket(reader);
    }

private:
    const ProxyManagerInterface* proxies_;
};

#endif
