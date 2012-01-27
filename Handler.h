#ifndef HANDLER_H_
#define HANDLER_H_

#include <QObject>
#include <QEvent>

#include "Hook.h"
#include "HookMessageEvent.h"
#include "EncryptedMessage.h"

class Handler: public QObject {
	Q_OBJECT

public:
	Handler() : QObject() {
	}

	bool event(QEvent* event) {
		if (event->type() == HookMessageEvent::EventType) {
			HookMessageEvent* hookEvent = (HookMessageEvent*) event;
			handleOutgoingMessage(hookEvent->message());
			return true;
		}
		return QObject::event(event);
	}

	virtual void handleOutgoingMessage(const EncryptedMessage*) = 0;
	virtual void handleIncomingMessage(const EncryptedMessage*) = 0;
};

#endif /* HANDLER_H_ */
