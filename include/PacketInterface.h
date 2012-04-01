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

#ifndef PACKETINTERFACE_H_
#define PACKETINTERFACE_H_

#include <QtGlobal>
#include <QObject>
#include <QString>

class PacketInterface: public QObject {
public:
	PacketInterface(QObject* parent = 0): QObject(parent) {}
	virtual ~PacketInterface() {}

	virtual quint16 length() const = 0;
	virtual const quint8* data() const = 0;

	virtual quint16 position() const = 0;
	virtual void setPosition(quint16) = 0;

	virtual void skip(quint16) = 0;
	virtual bool has(quint16) const = 0;

	virtual quint8 readU8() = 0;
	virtual quint16 readU16() = 0;
	virtual quint32 readU32() = 0;
	virtual quint64 readU64() = 0;
	virtual QString readString() = 0;
};

#endif
