#ifndef HOOKEVENT_H_
#define HOOKEVENT_H_

#include <QEvent>

#include "EncryptedMessage.h"

class HookMessageEvent: public QEvent {
public:
	enum Type {
		Outgoing,
		Incoming
	};

	static const QEvent::Type EventType = QEvent::User;

	HookMessageEvent(const Type, const EncryptedMessage*);

	const Type type() const;
	const EncryptedMessage* message() const;

private:
	const Type _type;
	const EncryptedMessage _message;
};

#endif /* HOOKEVENT_H_ */
