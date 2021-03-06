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

#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H

#include <QByteArray>
#include <QString>

#include <Packet.h>

#define DEFAULT_SIZE 64
#define MIN_EXTEND_SIZE 32

#define MAX(a, b) a > b ? a : b

class PacketBuilder {
public:
    PacketBuilder(quint16 length = DEFAULT_SIZE):
        data_(length, 0),
        position_(0),
        length_(0) {}

    inline Packet build() const {
        return Packet((const quint8*) data_.constData(), length_);
    }

    inline void skip(quint16 count) {
        reserve(count);
        position_ += count;
    }

    inline void writeU8(quint8 value) {
        write<quint8, 1>(value);
    }

    inline void writeU16(quint16 value) {
        write<quint16, 2>(value);
    }

    inline void writeU32(quint32 value) {
        write<quint32, 4>(value);
    }

    inline void writeU64(quint64 value) {
        write<quint64, 8>(value);
    }

    void writeString(const QString& value) {
        QByteArray array = value.toAscii();
        quint16 length = array.length();

        // Reserve header plus length bytes
        reserve(2 + length);

        // Write length
        writeU16(length);

        // Write raw data to the buffer
        memcpy(data_.data() + position_, array.constData(), length);
        position_ += length;
        length_ = MAX(length_, position_);
    }

private:
    QByteArray data_;
    quint16 position_;
    quint16 length_;

    inline void reserve(quint16 size) {
        quint16 newSize = position_ + size;
        if (data_.length() < newSize) {
            data_.resize(MAX(newSize, MIN_EXTEND_SIZE));
        }
    }

    template<typename T, int size>
    inline void write(T value) {
        reserve(size);

        *(T*) (data_.data() + position_) = value;
        position_ += size;
        length_ = MAX(length_, position_);
    }
};

#endif
