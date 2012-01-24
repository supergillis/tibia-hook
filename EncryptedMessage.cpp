#include <string.h>

#include "EncryptedMessage.h"
#include "DecryptedMessage.h"
#include "Encryption.h"

EncryptedMessage::EncryptedMessage() :
		_raw(NULL), _rawLength(0), _dataLength(0), _checksum(0), _needsMore(0) {
}

EncryptedMessage::EncryptedMessage(const uint8_t* buffer, uint16_t length) {
	_rawLength = length;
	_raw = new uint8_t[length];
	memcpy(_raw, buffer, length);

	_dataLength = *(uint16_t*) (_raw + HEADER_POSITION);
	_dataLength -= CHECKSUM_LENGTH;
	_checksum = *(uint32_t*) (_raw + CHECKSUM_POSITION);
	_needsMore = (_dataLength + DATA_POSITION) - _rawLength;
}

EncryptedMessage::EncryptedMessage(const EncryptedMessage& other) {
	_rawLength = other._rawLength;
	_raw = new uint8_t[other._rawLength];
	memcpy(_raw, other._raw, other._rawLength);

	_dataLength = other._dataLength;
	_checksum = other._checksum;
	_needsMore = other._needsMore;
}

EncryptedMessage::~EncryptedMessage() {
	if (_raw) {
		delete[] _raw;
	}
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

uint16_t EncryptedMessage::rawLength() const {
	return _rawLength;
}

const uint8_t* EncryptedMessage::rawData() const {
	return _raw;
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
EncryptedMessage EncryptedMessage::encrypt(const DecryptedMessage& message, const uint32_t key[]) {
	if (message.isValid()) {
		uint16_t length = message.rawLength();
		if (length % 8 != 0) {
			length += 8 - (length % 8);
		}

		uint8_t data[length + DATA_POSITION];
		memcpy(data + DATA_POSITION, message.rawData(), message.rawLength());

		if (Encryption::XTEA::encrypt(data + DATA_POSITION, length, key)) {
			*(uint16_t*) (data + HEADER_POSITION) = length + CHECKSUM_LENGTH;
			*(uint32_t*) (data + CHECKSUM_POSITION) = Encryption::Adler::checksum(data + DATA_POSITION, length);
			return EncryptedMessage(data, length + DATA_POSITION);
		}
	}
	return EncryptedMessage();
}
