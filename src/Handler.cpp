#include "Handler.h"
#include "Hook.h"
#include "ReceivingMessageEvent.h"

Handler::Handler(QObject* parent) :
		QObject(parent) {
}

bool Handler::event(QEvent* event) {
	if (event->type() == ReceivingMessageEvent::EventType) {
		ReceivingMessageEvent* hookEvent = (ReceivingMessageEvent*) event;
		switch (hookEvent->messageType()) {
			case ReceivingMessageEvent::Client:
				receiveFromClient(hookEvent->message());
				break;
			case ReceivingMessageEvent::Server:
				receiveFromServer(hookEvent->message());
				break;
		}
		return true;
	}
	return QObject::event(event);
}
