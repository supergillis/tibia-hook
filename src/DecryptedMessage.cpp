#include <string.h>

#include "DecryptedMessage.h"
#include "EncryptedMessage.h"

DecryptedMessage::DecryptedMessage() :
		Message(), _dataLength(0) {
}

DecryptedMessage::DecryptedMessage(const quint8* buffer, quint16 length) :
		Message() {
	initialize(buffer, length);
}

DecryptedMessage::DecryptedMessage(const Packet* packet) :
		Message() {
	quint16 length = packet->length();
	quint8 buffer[length + HEADER_LENGTH];
	*(quint16*) (buffer) = length;
	memcpy(buffer + HEADER_LENGTH, packet->data(), length);
	initialize(buffer, length + HEADER_LENGTH);
}

DecryptedMessage::DecryptedMessage(const EncryptedMessage* message) {
	if (message->isValid()) {
		quint16 length = message->length();
		quint8 data[length];
		memcpy(data, message->data(), length);

		if (Encryption::XTEA::decrypt(data, length, Encryption::XTEA::TIBIA_KEY)) {
			quint16 newLength = *(quint16*) data;
			if (newLength <= length) {
				initialize(data, HEADER_LENGTH + newLength);
			}
		}
	}
}

void DecryptedMessage::initialize(const quint8* buffer, quint16 length) {
	_dataLength = *(quint16*) buffer;
	Message::initialize(buffer, length);
}

bool DecryptedMessage::isValid() const {
	return _dataLength > 0;
}

quint16 DecryptedMessage::length() const {
	return _dataLength;
}

const quint8* DecryptedMessage::data() const {
	return rawData() + DATA_POSITION;
}
