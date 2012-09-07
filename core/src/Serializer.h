#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QtGlobal>
#include <QList>

#include "Packet.h"

struct UInt8Serializer {
	static void deserialize(quint8& value, Packet* packet) {
		value = packet->readU8();
	}
};

struct UInt16Serializer {
	static void deserialize(quint16& value, Packet* packet) {
		value = packet->readU16();
	}
};

struct UInt32Serializer {
	static void deserialize(quint32& value, Packet* packet) {
		value = packet->readU32();
	}
};

template<typename Type, typename MemberType, MemberType Type::*Member, typename Serializer>
struct ClassMemberSerializer {
	static void deserialize(Type& value, Packet* packet) {
		Serializer::deserialize(value.*Member, packet);
	}
};

template<typename Type, typename... Rest>
struct ClassSerializer;

template<typename Type, typename First, typename... Rest>
struct ClassSerializer<Type, First, Rest...> {
public:
	static inline void deserialize(Type& value, Packet* packet) {
		First::deserialize(value, packet);
		ClassSerializer<Type, Rest...>::deserialize(value, packet);
	}
};

template<typename Type>
struct ClassSerializer<Type> {
public:
	static inline void deserialize(Type&, Packet*) {}
};

#endif
