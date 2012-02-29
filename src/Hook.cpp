#include "Hook.h"
#include "DetourManager.h"

int Hook::argc_ = 0;

Hook::Hook() :
		QCoreApplication(Hook::argc_, NULL), handler_(NULL) {
	connect(DetourManager::instance(), SIGNAL(onClientMessage(QByteArray)), this, SLOT(receiveFromClient(const QByteArray&)));
	connect(DetourManager::instance(), SIGNAL(onServerMessage(QByteArray)), this, SLOT(receiveFromServer(const QByteArray&)));
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
	DetourManager::instance()->clientQueue()->enqueue(data);
}

/**
  * This function runs in the injected thread.
  */
void Hook::sendToServer(const QByteArray& data) {
	DetourManager::instance()->serverQueue()->enqueue(data);
}

/**
  * This function runs in the injected thread.
  */
void Hook::receiveFromClient(const QByteArray& data) {
	if (handler_ && handler_->receiveFromClient(data)) {
		sendToServer(data);
	}
}

/**
  * This function runs in the injected thread.
  */
void Hook::receiveFromServer(const QByteArray& data) {
	if (handler_) {
		handler_->receiveFromServer(data);
	}
}
