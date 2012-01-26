#include <string.h>

#include "DecryptedMessage.h"
#include "EncryptedMessage.h"

DecryptedMessage::DecryptedMessage() :
		Message(), _dataLength(0) {
}

DecryptedMessage::DecryptedMessage(const quint8* buffer, quint16 length) :
		Message(buffer, length) {
	_dataLength = *(quint16*) buffer;
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

DecryptedMessage DecryptedMessage::decrypt(const EncryptedMessage& message, const quint32 key[]) {
	if (message.isValid()) {
		quint16 length = message.length();
		quint8 data[length];
		memcpy(data, message.data(), length);

		if (Encryption::XTEA::decrypt(data, length, key)) {
			quint16 newLength = *(quint16*) data;
			if (newLength <= length) {
				return DecryptedMessage(data, HEADER_LENGTH + newLength);
			}
		}
	}
	return DecryptedMessage();
}
