#include "Message.h"

#include <QDebug>

QHash<quint8, Message*> Message::clientMessages_;
QHash<quint8, Message*> Message::serverMessages_;

void Message::registerClientMessage(Message* message) {
	clientMessages_.insert(message->type(), message);
}

void Message::registerServerMessage(Message* message) {
	serverMessages_.insert(message->type(), message);
}

Message* Message::lookupClientMessage(quint8 type) {
	return clientMessages_.value(type, NULL);
}

Message* Message::lookupServerMessage(quint8 type) {
	return serverMessages_.value(type, NULL);
}
