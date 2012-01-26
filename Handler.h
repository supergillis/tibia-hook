#ifndef HANDLER_H_
#define HANDLER_H_

#include <QObject>
#include <QEvent>

#include "Hook.h"
#include "EncryptedMessage.h"

class Handler: public QObject {
	Q_OBJECT

public:
	Handler(Hook* hook) : QObject(hook), _hook(hook) {
	}

	bool event(QEvent* event) {
		if (event->type() == OutgoingMessageEventType) {
			OutgoingMessageEvent* outputMessageEvent = (OutgoingMessageEvent*) event;
			handleOutgoingMessage(outputMessageEvent->message());
			return true;
		}
		return QObject::event(event);
	}

	inline Hook* hook() {
		return _hook;
	}

	virtual void handleOutgoingMessage(const EncryptedMessage&) = 0;
	virtual void handleIncomingMessage(const EncryptedMessage&) = 0;

private:
	Hook* _hook;
};

#endif /* HANDLER_H_ */
