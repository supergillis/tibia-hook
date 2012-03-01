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

#ifndef PACKET_H_
#define PACKET_H_

#include <QDebug>
#include <QObject>
#include <QString>

#define PACKET_END_OF_FILE "reached the end of the buffer"

class Packet {
public:
	Packet();
	virtual ~Packet() {};

	virtual quint16 length() const = 0;
	virtual const quint8* data() const = 0;

	quint16 position() const;
	void setPosition(quint16);

	void skip(quint16);
	bool has(quint16) const;

	quint8 readU8();
	quint16 readU16();
	quint32 readU32();
	quint64 readU64();
	QString readString();

protected:
	quint16 position_;

};

#endif /* PACKET_H_ */
