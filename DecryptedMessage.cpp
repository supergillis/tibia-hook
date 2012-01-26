#include <string.h>

#include "DecryptedMessage.h"
#include "EncryptedMessage.h"
#include "Encryption.h"

DecryptedMessage::DecryptedMessage() :
		Message(), _dataLength(0) {
}

DecryptedMessage::DecryptedMessage(const uint8_t* buffer, uint16_t length) :
		Message(buffer, length) {
	_dataLength = *(uint16_t*) buffer;
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
