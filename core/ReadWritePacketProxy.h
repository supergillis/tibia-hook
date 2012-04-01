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

#ifndef READWRITEPACKETPROXY_H
#define READWRITEPACKETPROXY_H

#include <QByteArray>
#include <QObject>
#include <QString>

#include <ReadWritePacketInterface.h>

#include "ReadWritePacket.h"

class ReadWritePacketProxy: public ReadWritePacketInterface {
	Q_OBJECT
	Q_PROPERTY(quint16 length READ length)
	Q_PROPERTY(quint16 position WRITE setPosition READ position)

public:
	ReadWritePacketProxy(): ReadWritePacketInterface(), packet_() {}

	virtual ~ReadWritePacketProxy() {}

	inline quint16 length() const { return packet_.length(); }
	inline const quint8* data() const { return packet_.data(); }

public slots:
	inline quint16 position() const { return packet_.position(); }
	inline void setPosition(quint16 position) { packet_.setPosition(position); }

	inline void skip(quint16 length) { packet_.skip(length); }
	inline bool has(quint16 length) const { return packet_.has(length); }

	inline quint8 readU8() { return packet_.readU8(); }
	inline quint16 readU16() { return packet_.readU16(); }
	inline quint32 readU32() { return packet_.readU32(); }
	inline quint64 readU64() { return packet_.readU64(); }
	inline QString readString() { return packet_.readString(); }

	inline void writeU8(quint8 value) { packet_.writeU8(value); }
	inline void writeU16(quint16 value) { packet_.writeU16(value); }
	inline void writeU32(quint32 value) { packet_.writeU32(value); }
	inline void writeU64(quint64 value) { packet_.writeU64(value); }
	inline void writeString(const QString& value) { packet_.writeString(value); }

private:
	ReadWritePacket packet_;
};

#endif
