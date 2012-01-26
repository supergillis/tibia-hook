#include <stdio.h>
#include <string.h>

#include "Hook.h"
#include "Main.h"
#include "Handler.h"
#include "Packet.h"
#include "ScriptHandler.h"

static int _argc = 0;

Hook::Hook() :
		QCoreApplication(_argc, NULL), _socket(-1), _loggedIn(true), _pendingLogin(false), _protocol(0) {
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
		Packet packet(message);
		quint8 protocol = packet.readU8();
		quint16 os = packet.readU16();
		quint16 client = packet.readU16();
		if (protocol == 0x0A) {
			_protocol = protocol;
			_pendingLogin = true;
		}
	}
	return write(buffer, length);
}

ssize_t Hook::hookIncomingMessage(quint8* buffer, ssize_t length) {
	return read(buffer, length);
}
