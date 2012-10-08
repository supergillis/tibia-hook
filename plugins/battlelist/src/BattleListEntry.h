#ifndef BATTLELISTENTRY_H
#define BATTLELISTENTRY_H

#include <BattleListEntryInterface.h>
#include <MemoryInterface.h>

class BattleListEntry: public BattleListEntryInterface {
public:
    BattleListEntry(MemoryInterface* memory, MemoryLocation base): memory_(memory), base_(base) {}

    inline quint32 id() const {
        return memory_->readUInt32(base_);
    }

    inline const char* rawName() const {
        return memory_->readRawString(base_ + 4);
    }

    inline QString name() const {
        return memory_->readString(base_ + 4);
    }

private:
    MemoryInterface* memory_;
    MemoryLocation base_;
};

#endif
