#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "hook.h"

static int _argc = 0;

Hook::Hook() :
		QCoreApplication(_argc, NULL), _socket(-1), _loggedIn(true), _pendingLogin(false), _key((uint32_t*) XTEA_START), _protocol(0) {
	_provider = new Provider(this);
	_provider->listen(QHostAddress::Any, 7170);
}

Hook::~Hook() {
	delete _provider;
}

const int Hook::socket() const {
	return _socket;
}

void Hook::setSocket(const int socket) {
	_socket = socket;
}

ssize_t Hook::read(uint8_t* buffer, ssize_t length) {
	return __read(_socket, buffer, length);
}

ssize_t Hook::write(const uint8_t* buffer, ssize_t length) {
	return __write(_socket, buffer, length);
}

ssize_t Hook::hookOutgoingPacket(const uint8_t* buffer, ssize_t length) {
	Message message(buffer, length);

	if (_loggedIn) {
		if (message.decrypt(_key)) {
			if (message.canRead()) {
				/*QCoreApplication::postEvent(_provider, new OutgoingMessageEvent(message), Qt::HighEventPriority);
				 return length;*/
				message.encrypt(_key);
			}
		}
	}
	else {
		uint8_t protocol = message.readByte();
		uint16_t os = message.readU16();
		uint16_t client = message.readU16();
		if (protocol == 0x0A) {
			_protocol = protocol;
			_pendingLogin = true;
		}
	}

	return write(buffer, length);
}

OutgoingMessageEvent::OutgoingMessageEvent(const Message& message) :
		QEvent(OutgoingMessageEventType), _message(message) {
}

ssize_t Hook::hookIncomingPacket(uint8_t* buffer, ssize_t length) {
	ssize_t read_length = read(buffer, length);

	/*Message message(buffer, read_length);

	 if (!message.decrypt(_key)) {
	 printf("[Hook::hookIncomingPacket] the packet could not be decrypted\n");
	 return read_length;
	 }

	 if (_loggedIn) {
	 // Do something with message
	 } else if (_pendingLogin) {
	 uint8_t byte = message.getByte();
	 if (byte == 0x14 || byte == 0x16) {
	 printf("Error while logging in!\n");
	 _pendingLogin = false;
	 } else {
	 // Rewind read byte
	 message.skipBytes(-1);
	 _loggedIn = true;
	 _pendingLogin = false;
	 // Do something with message
	 }
	 }*/

	return read_length;
}
