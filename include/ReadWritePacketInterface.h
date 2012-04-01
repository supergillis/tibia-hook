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

#ifndef READWRITEPACKETINTERFACE_H
#define READWRITEPACKETINTERFACE_H

#include <PacketInterface.h>

class ReadWritePacketInterface: public PacketInterface {
public:
	ReadWritePacketInterface(QObject* parent = 0): PacketInterface(parent) {}
	virtual ~ReadWritePacketInterface() {}

	virtual void writeU8(quint8) = 0;
	virtual void writeU16(quint16) = 0;
	virtual void writeU32(quint32) = 0;
	virtual void writeU64(quint64) = 0;
	virtual void writeString(const QString&) = 0;
};

#endif
