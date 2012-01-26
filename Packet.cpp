#include <string.h>

#include "Packet.h"
#include "Message.h"
#include "Encryption.h"

Packet::Packet() :
		_raw(), _dataLength(0) {
}

Packet::Packet(const uint8_t* buffer, uint16_t length) {
	_dataLength = *(uint16_t*) buffer;
	_raw = QByteArray((const char*) buffer, length);
}

bool Packet::isValid() const {
	return _dataLength > 0;
}

uint16_t Packet::length() const {
	return _dataLength;
}

const uint8_t* Packet::data() const {
	return rawData() + DATA_POSITION;
}

uint16_t Packet::rawLength() const {
	return _raw.length();
}

const uint8_t* Packet::rawData() const {
	return (const uint8_t*) _raw.constData();
}

Packet Packet::decrypt(const Message& message, const uint32_t key[]) {
	if (message.isValid()) {
		uint16_t length = message.length();
		uint8_t data[length];
		memcpy(data, message.data(), length);

		if (Encryption::XTEA::decrypt(data, length, key)) {
			uint16_t newLength = *(uint16_t*) data;
			if (newLength <= length) {
				return Packet(data, HEADER_LENGTH + newLength);
			}
		}
	}
	return Packet();
}
