#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <QDebug>

#include "message.h"
#include "encryption.h"

/*
 * [L L C C C C B B B B ...]
 * The length (L) of the message is stored in the first two bytes of the
 * buffer. This length includes the four checksum bytes.
 *
 * The actual buffer will thus be six bytes shorter (two of the length and four
 * of the checksum).
 *
 * Sometimes we receive a message that is not complete.
 */
Message::Message(const uint8_t* buffer, uint16_t length) {
	_length = (uint16_t) (buffer[0] | buffer[1] << 8) - 4;
	_checksum = (uint32_t) (buffer[2] | buffer[3] << 8 | buffer[4] << 16 | buffer[5] << 24);
	_position = 0;
	_needs_more = (_length + 6) - length;
	_buffer = QByteArray((char*) (buffer + 6), _length);
}

uint8_t* Message::data() {
	return (uint8_t*) _buffer.data();
}

const uint8_t* Message::data() const {
	return (const uint8_t*) _buffer.constData();
}

const QByteArray& Message::byteArray() const {
	return _buffer;
}

bool Message::decrypt(uint32_t key[]) {
	if (_needs_more != 0) {
		qWarning() << Q_FUNC_INFO << "we need" << _needs_more << "more bytes";
		return false;
	}
	if (!Encryption::XTEA::decrypt(_buffer, key)) {
		qWarning() << Q_FUNC_INFO << "unable to decrypt message";
		return false;
	}
	uint16_t decrypted_length = (uint16_t) (_buffer.at(0) | _buffer.at(1) << 8);
	if (decrypted_length > _length) {
		qWarning() << Q_FUNC_INFO << "decrypted length > original length";
		return false;
	}
	_buffer = _buffer.mid(2, decrypted_length);
	_length = decrypted_length;
	return true;
}

bool Message::encrypt(uint32_t key[]) {
	QByteArray encrypted = _buffer;
	encrypted.prepend((char) _length >> 8);
	encrypted.prepend((char) _length);
	if (encrypted.length() % 8 != 0) {
		int extra = 8 - (encrypted.length() % 8);
		while (extra-- > 0)
			encrypted.append((char) 0);
	}
	if (!Encryption::XTEA::encrypt(encrypted, key)) {
		qWarning() << Q_FUNC_INFO << "unable to encrypt message";
		return false;
	}
	_buffer = encrypted;
	_length = encrypted.length();
	return true;
}

uint16_t Message::length() const {
	return _buffer.length();
}

uint16_t Message::position() const {
	return _position;
}

void Message::setPosition(uint16_t value) {
	_position = value;
}

void Message::skip(int32_t value) {
	_position += value;
}

bool Message::canRead(uint16_t length) const {
	return (_position + length - 1 < _length);
}

uint8_t Message::readByte() throw () {
	if (!canRead(1))
		throw EndOfMessageException();
	return _buffer.at(_position++);
}

uint16_t Message::readU16() {
	return (uint16_t) (readByte() | readByte() << 8);
}

uint32_t Message::readU32() {
	return (uint32_t) (readByte() | readByte() << 8 | readByte() << 16 | readByte() << 24);
}
