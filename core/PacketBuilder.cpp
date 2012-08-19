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

#include "PacketBuilder.h"

#define max(a, b) a > b ? a : b

PacketBuilder::PacketBuilder() :
    data_(256, 0), length_(0), position_(0) {
}

PacketInterface* PacketBuilder::build() const {
    return new Packet((const quint8*) data_.data(), length_);
}

void PacketBuilder::reserve(quint16 size) {
    quint16 newSize = position_ + size;
    if (data_.length() < newSize) {
        data_.resize(newSize);
    }
}

void PacketBuilder::writeU8(quint8 value) {
    reserve(1);
    *(data_.data() + position_) = value;
    position_ += 1;
    length_ = max(length_, position_);
}

void PacketBuilder::writeU16(quint16 value) {
    reserve(2);
    *(quint16*) (data_.data() + position_) = value;
    position_ += 2;
    length_ = max(length_, position_);
}

void PacketBuilder::writeU32(quint32 value) {
    reserve(4);
    *(quint32*) (data_.data() + position_) = value;
    position_ += 4;
    length_ = max(length_, position_);
}

void PacketBuilder::writeU64(quint64 value) {
    reserve(8);
    *(quint64*) (data_.data() + position_) = value;
    position_ += 8;
    length_ = max(length_, position_);
}

void PacketBuilder::writeString(const QString& value) {
    QByteArray array = value.toAscii();
    quint16 length = array.length();

    writeU16(length);
    reserve(length);
    memcpy(data_.data() + position_, array.constData(), length);
    position_ += length;
    length_ = max(length_, position_);
}
