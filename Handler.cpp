#include "Handler.h"
#include "Hook.h"
#include "ReceivingMessageEvent.h"

Handler::Handler() :
		QObject() {
}

bool Handler::event(QEvent* event) {
	if (event->type() == ReceivingMessageEvent::EventType) {
		ReceivingMessageEvent* hookEvent = (ReceivingMessageEvent*) event;
		if (hookEvent->messageType() == ReceivingMessageEvent::Client) {
			receiveFromClient(hookEvent->message());
		}
		else {
			receiveFromServer(hookEvent->message());
		}
		return true;
	}
	return QObject::event(event);
}
