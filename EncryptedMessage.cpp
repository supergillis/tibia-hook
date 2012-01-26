#include <string.h>

#include "EncryptedMessage.h"
#include "DecryptedMessage.h"
#include "Encryption.h"

EncryptedMessage::EncryptedMessage() :
		Message(), _dataLength(0), _checksum(0), _needsMore(0) {
}

EncryptedMessage::EncryptedMessage(const uint8_t* buffer, uint16_t length) :
		Message(buffer, length) {
	_dataLength = *(uint16_t*) (buffer + HEADER_POSITION);
	_dataLength -= CHECKSUM_LENGTH;
	_checksum = *(uint32_t*) (buffer + CHECKSUM_POSITION);
	_needsMore = (_dataLength + DATA_POSITION) - length;
}

bool EncryptedMessage::isValid() const {
	return _dataLength > 0 && _needsMore == 0;
}

uint16_t EncryptedMessage::length() const {
	return _dataLength;
}

const uint8_t* EncryptedMessage::data() const {
	return rawData() + DATA_POSITION;
}

/**
 * To encrypt a message we first need to add some padding bytes. These are
 * filled completely random. Probably CIP is checking these padding bytes to
 * check if we're hacking... Problem for later? :p
 *
 * Then we can encrypt this message using XTEA.
 *
 * And finally we replace our buffer with the encrypted buffer and we also
 * replace our length with the new buffer length.
 */
EncryptedMessage EncryptedMessage::encrypt(const DecryptedMessage& packet, const uint32_t key[]) {
	if (packet.isValid()) {
		uint16_t length = packet.rawLength();
		if (length % 8 != 0) {
			length += 8 - (length % 8);
		}

		uint8_t data[length + DATA_POSITION];
		memcpy(data + DATA_POSITION, packet.rawData(), packet.rawLength());

		if (Encryption::XTEA::encrypt(data + DATA_POSITION, length, key)) {
			*(uint16_t*) (data + HEADER_POSITION) = length + CHECKSUM_LENGTH;
			*(uint32_t*) (data + CHECKSUM_POSITION) = Encryption::Adler::checksum(data + DATA_POSITION, length);
			return EncryptedMessage(data, length + DATA_POSITION);
		}
	}
	return EncryptedMessage();
}
