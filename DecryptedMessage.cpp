#include <string.h>

#include "DecryptedMessage.h"
#include "EncryptedMessage.h"
#include "Encryption.h"

DecryptedMessage::DecryptedMessage() :
		_raw(NULL), _raw_length(0), _data_length(0) {
}

DecryptedMessage::DecryptedMessage(const uint8_t* buffer, uint16_t length) {
	_raw_length = length;
	_raw = new uint8_t[length];
	memcpy(_raw, buffer, length);

	_data_length = *(uint16_t*) _raw;
}

DecryptedMessage::DecryptedMessage(const DecryptedMessage& other) {
	_raw_length = other._raw_length;
	_raw = new uint8_t[other._raw_length];
	memcpy(_raw, other._raw, other._raw_length);

	_data_length = other._data_length;
}

DecryptedMessage::~DecryptedMessage() {
	if (_raw) {
		delete[] _raw;
	}
}

bool DecryptedMessage::isValid() const {
	return _data_length > 0;
}

uint16_t DecryptedMessage::length() const {
	return _data_length;
}

const uint8_t* DecryptedMessage::data() const {
	return rawData() + DATA_POSITION;
}

uint16_t DecryptedMessage::rawLength() const {
	return _raw_length;
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
