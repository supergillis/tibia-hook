#ifndef SERVERPLAYERSTATUS_H
#define SERVERPLAYERSTATUS_H

#include "Message.h"
#include "MessageData.h"
#include "Packet.h"
#include "Serializer.h"

class ServerPlayerStatusData: public MessageData {
public:
	quint16 health;
	quint16 healthMax;
	quint32 capacity;
	quint64 experience;
	quint16 level;
	quint8 levelPercent;
	quint16 mana;
	quint16 manaMax;
	quint8 magicLevel;
	quint8 magicLevelPercent;
	quint8 soul;
	quint16 stamina;
};

class ServerPlayerStatus: public Message
{
	typedef ClassSerializer<
		ServerPlayerStatusData,
		ClassMemberSerializer<ServerPlayerStatusData, quint16, &ServerPlayerStatusData::health, UInt16Serializer>,
		ClassMemberSerializer<ServerPlayerStatusData, quint16, &ServerPlayerStatusData::healthMax, UInt16Serializer> > Serializer;

public:
	quint8 type() const { return 0xA0; }

	MessageData* deserialize(Packet*);
};

#endif
