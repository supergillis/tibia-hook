#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Hook.h"
#include "Main.h"

static int _argc = 0;

Hook::Hook() :
		QCoreApplication(_argc, NULL), _socket(-1), _loggedIn(true), _pendingLogin(false), _key((uint32_t*) XTEA_START), _protocol(0) {
	_hookServer = new HookServer(this);
	_hookServer->listen(QHostAddress::Any, 7170);
}

Hook::~Hook() {
	delete _hookServer;
}

const int Hook::socket() const {
	return _socket;
}

void Hook::setSocket(const int socket) {
	_socket = socket;
}

const uint32_t* Hook::key() const {
	return _key;
}

ssize_t Hook::read(uint8_t* buffer, ssize_t length) {
	return __read(_socket, buffer, length);
}

ssize_t Hook::write(const uint8_t* buffer, ssize_t length) {
	return __write(_socket, buffer, length);
}

ssize_t Hook::write(const Message& message) {
	return write(message.rawData(), message.rawLength());
}

ssize_t Hook::hookOutgoingPacket(const uint8_t* buffer, ssize_t length) {
	if (_loggedIn) {
		Message message(buffer, length);
		if (message.isValid()) {
			QCoreApplication::postEvent(_hookServer, new OutgoingMessageEvent(message), Qt::HighEventPriority);
			return length;
		}
	}
	else {
		Packet packet(buffer, length);
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

ssize_t Hook::hookIncomingPacket(uint8_t* buffer, ssize_t length) {
	return read(buffer, length);
}
