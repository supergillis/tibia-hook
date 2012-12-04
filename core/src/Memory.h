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

#ifndef MEMORY_H
#define MEMORY_H

#include <QtGlobal>
#include <QString>

#include <MemoryInterface.h>

class Memory: public MemoryInterface {
public:
    inline quint8 readUInt8(MemoryLocation location) const {
        return read<quint8>(location);
    }

    inline quint16 readUInt16(MemoryLocation location) const {
        return read<quint16>(location);
    }

    inline quint32 readUInt32(MemoryLocation location) const {
        return read<quint32>(location);
    }

    inline quint64 readUInt64(MemoryLocation location) const {
        return read<quint64>(location);
    }

    inline const char* readRawString(MemoryLocation location) const {
        return (const char*) location;
    }

    inline QString readString(MemoryLocation location) const {
        return QString(readRawString(location));
    }

    MemoryLocation rebase(MemoryLocation address) const;
    static MemoryLocation staticRebase(MemoryLocation address);

private:
    template <typename T>
    inline T read(MemoryLocation location) const {
        return *((T*) location);
    }
};

#endif
