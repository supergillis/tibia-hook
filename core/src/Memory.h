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

    MemoryLocation mapAddress(MemoryLocation address) const;
    static MemoryLocation staticMapAddress(MemoryLocation address);

private:
    template <typename T>
    inline T read(MemoryLocation location) const {
        return *((T*) location);
    }
};

#endif
