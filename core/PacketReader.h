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

#ifndef PACKETREADER_H
#define PACKETREADER_H

#include <PacketInterface.h>

#define PACKET_END_OF_FILE "reached the end of the buffer"

class PacketReader {
public:
    PacketReader(const PacketInterface* packet): packet_(packet), position_(0) {}
    virtual ~PacketReader() {}

    const PacketInterface* packet() const;

    inline quint16 position() const {
        return position_;
    }

    inline void setPosition(quint16 position) {
        position_ = position;
    }

    void skip(quint16 count) {
        position_ += count;
    }

    inline bool has(quint16 count) const {
        return packet_->length() - position_ >= count;
    }

    inline quint8 readU8() {
        return read<quint8, 1>();
    }

    inline quint16 readU16() {
        return read<quint16, 2>();
    }

    inline quint32 readU32() {
        return read<quint32, 4>();
    }

    inline quint64 readU64() {
        return read<quint64, 8>();
    }

    QString readString() {
        quint16 length = readU16();
        if (has(length)) {
            QString value = QString::fromAscii((const char*) (packet_->data() + position_), length);
            position_ += length;
            return value;
        }
        qWarning() << PACKET_END_OF_FILE;
        return QString();
    }

protected:
    const PacketInterface* packet_;
    quint16 position_;

private:
    template<typename T, int size>
    inline T read() {
        if (!has(size)) {
            qWarning() << PACKET_END_OF_FILE;
            return 0;
        }
        T value = *((T*) (packet_->data() + position_));
        position_ += size;
        return value;
    }
};

#endif // PACKETREADER_H
