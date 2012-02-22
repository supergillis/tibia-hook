#include "Hook.h"
#include "DetourManager.h"
#include "ReadOnlyPacket.h"

static int argc_ = 0;

Hook::Hook() :
		QApplication(argc_, NULL), handler_(NULL) {
	DetourManager::initialize(this);
	QMainWindow* window = new QMainWindow();
	window->show();
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
 * This function runs in the Tibia thread.
 */
bool Hook::receiveFromClient(const QByteArray& data) {
	return handler_->receiveFromClient(data);
}

/**
 * This function runs in the Tibia thread.
 */
void Hook::receiveFromServer(const QByteArray& data) {
	handler_->receiveFromServer(data);
}
