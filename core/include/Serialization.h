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

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QtGlobal>

#include <PacketBuilder.h>
#include <PacketReader.h>
#include <Position.h>

namespace s11n {

class Container {
public:
    virtual void skip(quint16 count) = 0;

    virtual void uint8(quint8& value) = 0;
    virtual void uint16(quint16& value) = 0;
    virtual void uint32(quint32& value) = 0;
    virtual void uint64(quint64& value) = 0;
    virtual void string(QString& value) = 0;
};

class PacketBuilderContainer: public Container {
public:
    PacketBuilderContainer(PacketBuilder& builder): builder_(builder) {}

    void skip(quint16 count) {
        builder_.skip(count);
    }

    void uint8(quint8& value) {
        builder_.writeU8(value);
    }

    void uint16(quint16& value) {
        builder_.writeU16(value);
    }

    void uint32(quint32& value) {
        builder_.writeU32(value);
    }

    void uint64(quint64& value) {
        builder_.writeU64(value);
    }

    void string(QString& value) {
        builder_.writeString(value);
    }

private:
    PacketBuilder& builder_;
};

class PacketReaderContainer: public Container {
public:
    PacketReaderContainer(PacketReader& reader): reader_(reader) {}

    void skip(quint16 count) {
        reader_.skip(count);
    }

    void uint8(quint8& value) {
        value = reader_.readU8();
    }

    void uint16(quint16& value) {
        value = reader_.readU16();
    }

    void uint32(quint32& value) {
        value = reader_.readU32();
    }

    void uint64(quint64& value) {
        value = reader_.readU64();
    }

    void string(QString& value) {
        value = reader_.readString();
    }

private:
    PacketReader& reader_;
};

template<class Type>
void serialization(Type& object, Container& container) {
    object.serialization(container);
}

template<>
void serialization(quint8& object, Container& container) {
    container.uint8(object);
}

template<>
void serialization(quint16& object, Container& container) {
    container.uint16(object);
}

template<>
void serialization(quint32& object, Container& container) {
    container.uint32(object);
}

template<>
void serialization(quint64& object, Container& container) {
    container.uint64(object);
}

template<>
void serialization(QString& object, Container& container) {
    container.string(object);
}

template<>
void serialization(Position& object, Container& container) {
    serialization(object.x, container);
    serialization(object.y, container);
    serialization(object.z, container);
}

template<class Type>
void serialize(Type& object, PacketBuilder& builder) {
    PacketBuilderContainer container(builder);
    serialization(object, container);
}

template<class Type>
void deserialize(Type& object, PacketReader& reader) {
    PacketReaderContainer container(reader);
    serialization(object, container);
}

}

#endif
