#include <string.h>

#include "DecryptedMessage.h"
#include "EncryptedMessage.h"
#include "Encryption.h"

DecryptedMessage::DecryptedMessage() :
		_raw(NULL), _rawLength(0), _dataLength(0) {
}

DecryptedMessage::DecryptedMessage(const uint8_t* buffer, uint16_t length) {
	_rawLength = length;
	_raw = new uint8_t[length];
	memcpy(_raw, buffer, length);

	_dataLength = *(uint16_t*) _raw;
}

DecryptedMessage::DecryptedMessage(const DecryptedMessage& other) {
	_rawLength = other._rawLength;
	_raw = new uint8_t[other._rawLength];
	memcpy(_raw, other._raw, other._rawLength);

	_dataLength = other._dataLength;
}

DecryptedMessage::~DecryptedMessage() {
	if (_raw) {
		delete[] _raw;
	}
}

bool DecryptedMessage::isValid() const {
	return _dataLength > 0;
}

uint16_t DecryptedMessage::length() const {
	return _dataLength;
}

const uint8_t* DecryptedMessage::data() const {
	return rawData() + DATA_POSITION;
}

uint16_t DecryptedMessage::rawLength() const {
	return _rawLength;
}

const uint8_t* DecryptedMessage::rawData() const {
	return (const uint8_t*) _raw;
}

DecryptedMessage DecryptedMessage::decrypt(const EncryptedMessage& message, const uint32_t key[]) {
	if (message.isValid()) {
		uint16_t length = message.length();
		uint8_t data[length];
		memcpy(data, message.data(), length);

		if (Encryption::XTEA::decrypt(data, length, key)) {
			uint16_t newLength = *(uint16_t*) data;
			if (newLength <= length) {
				return DecryptedMessage(data, HEADER_LENGTH + newLength);
			}
		}
	}
	return DecryptedMessage();
}
