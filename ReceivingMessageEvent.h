#ifndef HOOKEVENT_H_
#define HOOKEVENT_H_

#include <QEvent>

#include "EncryptedMessage.h"

class ReceivingMessageEvent: public QEvent {
public:
	static const QEvent::Type EventType = QEvent::User;

	enum MessageType {
		Client,
		Server
	};

	ReceivingMessageEvent(const MessageType, const EncryptedMessage*);

	const MessageType messageType() const;
	const EncryptedMessage* message() const;

private:
	const MessageType _type;
	const EncryptedMessage _message;
};

#endif /* HOOKEVENT_H_ */
