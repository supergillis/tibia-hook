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

#ifndef READONLYPACKET_H
#define READONLYPACKET_H

#include <QByteArray>
#include <QObject>
#include <QString>

#include "Packet.h"

class ReadOnlyPacket: public Packet {
public:
	ReadOnlyPacket(const QByteArray&);
	ReadOnlyPacket(const quint8*, quint16);

	quint16 length() const;
	const quint8* data() const;

private:
	QByteArray raw_;
};

#endif
