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

#ifndef PACKET_H
#define PACKET_H

#include <QtGlobal>
#include <QByteArray>

class Packet {
public:
    Packet(const QByteArray& data): raw_(data) {}
    Packet(const quint8* buffer, quint16 length): raw_((const char*) buffer, length) {}

    quint16 length() const {
        return raw_.length();
    }

    const quint8* rawData() const {
        return (const quint8*) raw_.data();
    }

    const QByteArray& data() const {
        return raw_;
    }

private:
    QByteArray raw_;
};

#endif
