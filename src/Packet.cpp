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

#include "Packet.h"

Packet::Packet() :
		position_(0) {
}

quint16 Packet::position() const {
	return position_;
}

void Packet::setPosition(quint16 position) {
	position_ = position;
}

void Packet::skip(quint16 count) {
	position_ += count;
}

bool Packet::has(quint16 count) const {
	return length() - position_ >= count;
}

quint8 Packet::readU8() {
	if (has(1)) {
		quint8 value = *(data() + position_);
		position_ += 1;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint16 Packet::readU16() {
	if (has(2)) {
		quint16 value = *(quint16*) (data() + position_);
		position_ += 2;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint32 Packet::readU32() {
	if (has(4)) {
		quint32 value = *(quint32*) (data() + position_);
		position_ += 4;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint64 Packet::readU64() {
	if (has(8)) {
		quint64 value = *(quint64*) (data() + position_);
		position_ += 8;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

QString Packet::readString() {
	quint16 length = readU16();
	if (has(length)) {
		QString value = QString::fromAscii((const char*) (data() + position_), length);
		position_ += length;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return QString();
}
