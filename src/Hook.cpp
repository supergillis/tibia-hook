#include <stdio.h>
#include <string.h>

#include <QDebug>

#include "Hook.h"
#include "ReadOnlyPacket.h"
#include "Main.h"

static int argc_ = 0;

Hook::Hook() :
		QCoreApplication(argc_, NULL), handler_(NULL) {
}

Handler* Hook::handler() {
	return handler_;
}

void Hook::setHandler(Handler* handler) {
	handler_ = handler;
}

void Hook::sendToClient(const Packet* packet) {
}

void Hook::sendToServer(const Packet* packet) {
	// Save length and buffer
	quint16 length_backup = *((quint32*) ADDRESS_BUFFER_LENGTH);
	quint8* buffer_backup[length_backup];
	memcpy(buffer_backup, (quint8*) ADDRESS_BUFFER, length_backup);

	// Replace length and buffer with packet length and buffer
	*((quint32*) ADDRESS_BUFFER_LENGTH) = packet->length() + 8;
	memset((quint8*) ADDRESS_BUFFER, 0, 8);
	memcpy((quint8*) ADDRESS_BUFFER + 8, packet->data(), packet->length());
	writePacket(true);

	// Restore length and buffer
	*((quint32*) ADDRESS_BUFFER_LENGTH) = length_backup;
	memcpy((quint8*) ADDRESS_BUFFER, buffer_backup, length_backup);
}

void Hook::receiveFromClient(bool encrypt) {
	// Create a packet
	quint8* buffer = (quint8*) (ADDRESS_BUFFER + 8);
	quint32 length = *((quint32*) ADDRESS_BUFFER_LENGTH) - 8;
	ReadOnlyPacket packet(buffer, length);
	// If we don't hook this packet, then it
	if (!handler_->receiveFromClient(&packet)) {
		writePacket(encrypt);
	}
}

void Hook::receiveFromServer() {
}
