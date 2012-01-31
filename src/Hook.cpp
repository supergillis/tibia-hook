#include <stdio.h>
#include <string.h>

#include <QDebug>

#include "Hook.h"
#include "MessageEvent.h"
#include "ReadOnlyPacket.h"

static int argc_ = 0;

Hook::Hook() :
		QCoreApplication(argc_, NULL), socket_(NULL), client_(NULL), handler_(NULL), loggedIn_(false), pendingLogin_(false) {
}

HookSocket* Hook::socket() {
	return socket_;
}

void Hook::setSocket(HookSocket* socket) {
	socket_ = socket;
}

HookClient* Hook::client() {
	return client_;
}

void Hook::setClient(HookClient* client) {
	client_ = client;
}

Handler* Hook::handler() {
	return handler_;
}

void Hook::setHandler(Handler* handler) {
	handler_ = handler;
}

int Hook::pendingClientMessages() const {
	return queue_.size();
}

int Hook::pendingClientEvents() const {
	return client_->pendingEvents();
}

void Hook::sendKeyPress(int code) {
	client_->sendKeyPress(code);
}

void Hook::sendToClient(const quint8* buffer, ssize_t length) {
	queue_.enqueue(QByteArray((const char*) buffer, length));
}

void Hook::sendToClient(const EncryptedMessage* message) {
	if (message->isValid()) {
		sendToClient(message->rawData(), message->rawLength());
	}
}

void Hook::sendToClient(const DecryptedMessage* message) {
	EncryptedMessage encrypted(message);
	sendToClient(&encrypted);
}

ssize_t Hook::sendToServer(const quint8* buffer, ssize_t length) {
	return socket_->write(buffer, length);
}

ssize_t Hook::sendToServer(const EncryptedMessage* message) {
	return sendToServer(message->rawData(), message->rawLength());
}

ssize_t Hook::sendToServer(const DecryptedMessage* message) {
	EncryptedMessage encrypted(message);
	if (encrypted.isValid()) {
		return sendToServer(&encrypted);
	}
	return 0;
}

ssize_t Hook::receiveFromClient(const quint8* buffer, ssize_t length) {
	EncryptedMessage message(buffer, length);
	if (message.isValid()) {
		QCoreApplication::postEvent(handler_, new MessageEvent(MessageEvent::ReceivingClient, &message), Qt::HighEventPriority);
		return length;
	}
	return sendToServer(buffer, length);
}

ssize_t Hook::receiveFromServer(quint8* buffer, ssize_t length) {
	if (pendingClientMessages() > 0) {
		QByteArray data = queue_.dequeue();
		memcpy(buffer, data.constData(), data.length());
		return data.length();
	}

	ssize_t result = socket_->read(buffer, length);
	EncryptedMessage message(buffer, result);
	if (message.isValid()) {
		QCoreApplication::postEvent(handler_, new MessageEvent(MessageEvent::ReceivingServer, &message), Qt::HighEventPriority);
	}
	return result;
}
