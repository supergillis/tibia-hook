#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "hook.h"

static int _argc = 0;

Hook::Hook() :
		QCoreApplication(_argc, NULL), _loggedIn(false), _pendingLogin(false), _key((uint32_t*) XTEA_START) {
	_provider = new Provider();
	_provider->listen(QHostAddress::Any, 7170);
}

Hook::~Hook() {
	_loggedIn = false;
	_pendingLogin = false;
	delete _provider;
}

ssize_t Hook::hookOutgoingPacket(int socket, const uint8_t* buffer, ssize_t length) {
	Message message(buffer, length);

	bool send = true;
	if (_loggedIn) {
		if (message.decrypt(_key)) {
			if (message.canRead()) {
				// Do something with message
			}
		} else {
			printf("[Hook::hookOutgoingPacket] could not decrypt packet");
		}
	} else {
		uint8_t protocol = message.getByte();
		uint16_t os = message.getU16();
		uint16_t client = message.getU16();
		if (protocol == 0x0A) {
			_protocol = protocol;
			_pendingLogin = true;
		}
	}

	if (!send)
		return length;

	return __write(socket, buffer, length);
}

ssize_t Hook::hookIncomingPacket(int socket, uint8_t* buffer, ssize_t length) {
	ssize_t read_length = __read(socket, buffer, length);

	Message message(buffer, read_length);

	if (!message.decrypt(_key)) {
		printf("[Hook::hookIncomingPacket] the packet could not be decrypted\n");
		return read_length;
	}

	if (_loggedIn) {
		// Do something with message
	} else if (_pendingLogin) {
		uint8_t byte = message.getByte(true);
		if (byte == 0x14 || byte == 0x16) {
			message.getByte(); // remove the type byte we peeked
			printf("Error while logging in!\n");
			printf("Message: %s\n", message.getString().c_str());
			if (byte == 0x16)
				printf("Time: %d\n", message.getByte());
			_pendingLogin = false;
		} else {
			_loggedIn = true;
			_pendingLogin = false;
			// Do something with message
		}
	}

	return read_length;
}
