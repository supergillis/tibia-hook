#include <string.h>

#include "Packet.h"
#include "Message.h"
#include "Encryption.h"

Packet::Packet() :
		_raw(NULL), _rawLength(0), _dataLength(0) {
}

Packet::Packet(const uint8_t* buffer, uint16_t length) {
	_rawLength = length;
	_raw = new uint8_t[length];
	memcpy(_raw, buffer, length);

	_dataLength = *(uint16_t*) _raw;
}

Packet::Packet(const Packet& other) {
	_rawLength = other._rawLength;
	_raw = new uint8_t[other._rawLength];
	memcpy(_raw, other._raw, other._rawLength);

	_dataLength = other._dataLength;
}

Packet::~Packet() {
	if (_raw) {
		delete[] _raw;
	}
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
	return _rawLength;
}

const uint8_t* Packet::rawData() const {
	return (const uint8_t*) _raw;
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
