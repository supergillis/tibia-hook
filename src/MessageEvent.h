#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H

#include <QEvent>

#include "EncryptedMessage.h"

class MessageEvent: public QEvent {
public:
	static const QEvent::Type EventType = QEvent::User;

	enum MessageType {
		ReceivingClient,
		ReceivingServer
	};

	MessageEvent(const MessageType, const EncryptedMessage*);

	const MessageType messageType() const;
	const EncryptedMessage* message() const;

private:
	const MessageType type_;
	const EncryptedMessage message_;
};

#endif /* MESSAGEEVENT_H */
