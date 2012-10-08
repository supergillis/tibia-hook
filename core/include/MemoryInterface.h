#ifndef MEMORYINTERFACE_H
#define MEMORYINTERFACE_H

#include <QtGlobal>
#include <QString>

typedef quint64 MemoryLocation;

class MemoryInterface {
public:
    virtual MemoryLocation mapAddress(MemoryLocation address) const = 0;

    virtual quint8 readUInt8(MemoryLocation location) const = 0;
    virtual quint16 readUInt16(MemoryLocation location) const = 0;
    virtual quint32 readUInt32(MemoryLocation location) const = 0;
    virtual quint64 readUInt64(MemoryLocation location) const = 0;

    virtual const char* readRawString(MemoryLocation location) const = 0;
    virtual QString readString(MemoryLocation location) const = 0;
};

#endif
