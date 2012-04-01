#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtGlobal>
#include <QHash>

#include "MessageData.h"
#include "Packet.h"

class Message
{
public:
	virtual ~Message() {}

	virtual quint8 type() const = 0;
	virtual MessageData* deserialize(Packet*) = 0;

	static void registerClientMessage(Message* message);
	static void registerServerMessage(Message* message);

	static Message* lookupClientMessage(quint8 type);
	static Message* lookupServerMessage(quint8 type);

private:
	static QHash<quint8, Message*> clientMessages_;
	static QHash<quint8, Message*> serverMessages_;
};

#endif
