#include "Hook.h"
#include "DataEvent.h"
#include "DetourManager.h"
#include "ReadOnlyPacket.h"

static int argc_ = 0;

Hook::Hook() :
		QApplication(argc_, NULL), handler_(NULL) {
	DetourManager::initialize(this);
}

bool Hook::event(QEvent* event) {
	if (event->type() == (QEvent::Type) DataEvent::Client || event->type() == (QEvent::Type) DataEvent::Server) {
		DataEvent* dataEvent = (DataEvent*) event;
		switch (dataEvent->type()) {
			case DataEvent::Client:
				receiveFromClient(dataEvent->data());
				break;
			case DataEvent::Server:
				receiveFromServer(dataEvent->data());
				break;
		}
	}
	return QApplication::event(event);
}

Handler* Hook::handler() {
	return handler_;
}

void Hook::setHandler(Handler* handler) {
	handler_ = handler;
}

/**
 * This function runs in the injected thread.
 */
void Hook::sendToClient(const QByteArray& data) {
	DetourManager::clientQueue()->enqueue(data);
}

/**
 * This function runs in the injected thread.
 */
void Hook::sendToServer(const QByteArray& data) {
	DetourManager::serverQueue()->enqueue(data);
}

/**
 * This function runs in the injected thread.
 */
void Hook::receiveFromClient(const QByteArray& data) {
	qDebug() << "receiveFromClient";
	if (handler_->receiveFromClient(data)) {
		DetourManager::serverQueue()->enqueue(data);
	}
}

/**
 * This function runs in the injected thread.
 */
void Hook::receiveFromServer(const QByteArray& data) {
	handler_->receiveFromServer(data);
}
