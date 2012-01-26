#include <stdio.h>
#include <string.h>

#include "Hook.h"
#include "Main.h"
#include "Handler.h"
#include "ServerHandler.h"
#include "ScriptHandler.h"
#include "Encryption.h"

static int _argc = 0;

Hook::Hook() :
		QCoreApplication(_argc, NULL), _socket(-1), _loggedIn(true), _pendingLogin(false), _protocol(0) {
	//ServerHandler* serverHandler = new ServerHandler(this);
	//serverHandler->listen(QHostAddress::Any, 7170);
	_handler = new ScriptHandler(this);
}

Hook::~Hook() {
	delete _handler;
}

const int Hook::socket() const {
	return _socket;
}

void Hook::setSocket(const int socket) {
	_socket = socket;
}

ssize_t Hook::read(quint8* buffer, ssize_t length) {
	return __read(_socket, buffer, length);
}

ssize_t Hook::write(const quint8* buffer, ssize_t length) {
	return __write(_socket, buffer, length);
}

ssize_t Hook::write(const EncryptedMessage& message) {
	return write(message.rawData(), message.rawLength());
}

ssize_t Hook::hookOutgoingMessage(const quint8* buffer, ssize_t length) {
	if (_loggedIn) {
		EncryptedMessage message(buffer, length);
		if (message.isValid()) {
			QCoreApplication::postEvent(_handler, new OutgoingMessageEvent(message), Qt::HighEventPriority);
			return length;
		}
	}
	else {
		DecryptedMessage message(buffer, length);
		/*uint8_t protocol = message.readByte();
		 uint16_t os = message.readU16();
		 uint16_t client = message.readU16();
		 if (protocol == 0x0A) {
		 _protocol = protocol;
		 _pendingLogin = true;
		 }*/
	}
	return write(buffer, length);
}

ssize_t Hook::hookIncomingMessage(quint8* buffer, ssize_t length) {
	return read(buffer, length);
}
