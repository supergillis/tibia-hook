#include "Handler.h"
#include "Hook.h"
#include "MessageEvent.h"

Handler::Handler(QObject* parent) :
		QObject(parent) {
}

bool Handler::event(QEvent* event) {
	if (event->type() == MessageEvent::EventType) {
		MessageEvent* hookEvent = (MessageEvent*) event;
		switch (hookEvent->messageType()) {
			case MessageEvent::ReceivingClient:
				receiveFromClient(hookEvent->message());
				break;
			case MessageEvent::ReceivingServer:
				receiveFromServer(hookEvent->message());
				break;
		}
		return true;
	}
	return QObject::event(event);
}
