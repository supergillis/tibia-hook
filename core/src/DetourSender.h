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

#ifndef DETOURSENDER_H
#define DETOURSENDER_H

#include <SenderInterface.h>

#include "DetourManager.h"
#include "PacketBuilder.h"

class DetourSender: public SenderInterface {
public:
    DetourSender(): SenderInterface() {}

    inline void sendToClient(const QByteArray& data) {
        DetourManager::clientQueue()->enqueue(data);
    }

    inline void sendToClient(const Packet& packet) {
        DetourManager::clientQueue()->enqueue(packet.data());
    }

    inline void sendToServer(const QByteArray& data) {
       DetourManager::serverQueue()->enqueue(data);
    }

    inline void sendToServer(const Packet& packet) {
        DetourManager::serverQueue()->enqueue(packet.data());
    }

private:
	DetourSender(const DetourSender&);
    DetourSender& operator=(const DetourSender&);
};

#endif
