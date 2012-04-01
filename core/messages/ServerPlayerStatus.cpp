#include "ServerPlayerStatus.h"

#include <QDebug>

MessageData* ServerPlayerStatus::deserialize(Packet* packet) {
	ServerPlayerStatusData* data = new ServerPlayerStatusData;
	Serializer::deserialize(*data, packet);
	return data;
}
