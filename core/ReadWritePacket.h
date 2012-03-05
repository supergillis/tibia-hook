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

#ifndef READWRITEPACKET_H_
#define READWRITEPACKET_H_

#include <QByteArray>
#include <QObject>
#include <QString>

#include <ReadWritePacketInterface.h>

#include "Packet.h"

class ReadWritePacket: public Packet {
public:
	ReadWritePacket();

	quint16 length() const;
	const quint8* data() const;

	void writeU8(quint8);
	void writeU16(quint16);
	void writeU32(quint32);
	void writeU64(quint64);
	void writeString(const QString&);

private:
	void reserve(quint16);

	QByteArray data_;
	quint16 length_;
};

#endif /* READWRITEPACKET_H_ */
