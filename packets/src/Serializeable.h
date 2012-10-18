#ifndef SERIALIZEABLE_H
#define SERIALIZEABLE_H

#include <PacketBuilder.h>
#include <PacketReader.h>

class Serializeable {
public:
    virtual void serialize(PacketBuilder&) = 0;
    virtual void deserialize(PacketReader&) = 0;
};

template<typename Self, typename Serializer>
class SerializeableBase: public Serializeable {
public:
    inline void serialize(PacketBuilder& builder) {
        Serializer::serialize((Self&) *this, builder);
    }

    inline void deserialize(PacketReader& reader) {
        Serializer::deserialize((Self&) *this, reader);
    }

    static inline void serialize(Self& instance, PacketBuilder& builder) {
        Serializer::serialize(instance, builder);
    }

    static inline void deserialize(Self& instance, PacketReader& reader) {
        Serializer::deserialize(instance, reader);
    }
};

#endif
