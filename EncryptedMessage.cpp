#include <string.h>

#include "EncryptedMessage.h"
#include "DecryptedMessage.h"
#include "Encryption.h"

EncryptedMessage::EncryptedMessage() :
		_raw(NULL), _raw_length(0), _data_length(0), _checksum(0), _needs_more(0) {
}

EncryptedMessage::EncryptedMessage(const uint8_t* buffer, uint16_t length) {
	_raw_length = length;
	_raw = new uint8_t[length];
	memcpy(_raw, buffer, length);

	_data_length = *(uint16_t*) (_raw + HEADER_POSITION);
	_data_length -= CHECKSUM_LENGTH;
	_checksum = *(uint32_t*) (_raw + CHECKSUM_POSITION);
	_needs_more = (_data_length + DATA_POSITION) - _raw_length;
}

EncryptedMessage::EncryptedMessage(const EncryptedMessage& other) {
	_raw_length = other._raw_length;
	_raw = new uint8_t[other._raw_length];
	memcpy(_raw, other._raw, other._raw_length);

	_data_length = other._data_length;
	_checksum = other._checksum;
	_needs_more = other._needs_more;
}

EncryptedMessage::~EncryptedMessage() {
	if (_raw) {
		delete[] _raw;
	}
}

bool EncryptedMessage::isValid() const {
	return _data_length > 0 && _needs_more == 0;
}

uint16_t EncryptedMessage::length() const {
	return _data_length;
}

const uint8_t* EncryptedMessage::data() const {
	return rawData() + DATA_POSITION;
}

uint16_t EncryptedMessage::rawLength() const {
	return _raw_length;
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
		if (length % 8 != 0)
			length += 8 - (length % 8);

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
