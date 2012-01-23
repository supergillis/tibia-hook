#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "message.h"

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
	_buffer = new unsigned char[_length];
	memcpy(_buffer, buffer + 6, _length);
}

Message::Message(const Message& other) {
	_length = other._length;
	_checksum = other._checksum;
	_position = other._position;
	_needs_more = other._needs_more;
	_buffer = new unsigned char[_length];
	memcpy(_buffer, other._buffer, _length);
}

Message::~Message() {
	delete _buffer;
}

bool Message::paste(const uint8_t* buffer, uint16_t length) {
	if (_needs_more == 0) {
		printf("[Message::paste] we don't need more data!\n");
		return false;
	}
	if (_needs_more != length) {
		printf("[Message::paste] we need %d data, we got %d\n", _needs_more, length);
		return false;
	}
	memcpy(&_buffer[_length], buffer, length);
	return true;
}

bool Message::decrypt(uint32_t key[]) {
	if (_needs_more > 0) {
		printf("[Message::decrypt] couldn't decrypt: we need %d more bytes!\n", _needs_more);
		return false;
	}
	if (_length % 8 != 0) {
		printf("[Message::decrypt] couldn't decrypt: %d mod 8 != 0\n", _length);
		return false;
	}
	unsigned int buffer_length = _length / 4;
	unsigned int read_pos = 0;
	unsigned int* buffer = (unsigned int*) _buffer;
	while (read_pos < buffer_length) {
		unsigned int v0 = buffer[read_pos], v1 = buffer[read_pos + 1];
		unsigned int delta = 0x61C88647;
		unsigned int sum = 0xC6EF3720;
		for (int i = 0; i < 32; i++) {
			v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + key[sum >> 11 & 3]);
			sum += delta;
			v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + key[sum & 3]);
		}
		buffer[read_pos] = v0;
		buffer[read_pos + 1] = v1;
		read_pos = read_pos + 2;
	}
	uint32_t new_length = (uint16_t) (_buffer[0] | _buffer[1] << 8);
	if (new_length > _length) {
		printf("[Message::decrypt] couldn't decrypt: decrypted length > original length\n");
		return false;
	}
	_length = (uint16_t) (_buffer[0] | _buffer[1] << 8);
	_position = 0;
	memmove(_buffer, _buffer + 2, _length);
	return true;
}

uint16_t Message::getBufferLength() {
	return _length;
}

uint16_t Message::getBufferPosition() {
	return _position;
}

void Message::setBufferPosition(uint16_t value) {
	_position = value;
}

void Message::skipBytes(uint16_t value) {
	_position += value;
}

bool Message::canRead(uint16_t length) {
	return (_position + length - 1 < _length);
}

uint8_t Message::getByte(bool peek) {
	if (!canRead(1)) {
		printf("[Message::getByte] couldn't get byte: reached end of buffer\n");
		return 0;
	}
	uint8_t value = _buffer[_position];
	if (!peek)
		_position += 1;
	return value;
}

uint16_t Message::getU16(bool peek) {
	if (!canRead(2)) {
		printf("[Message::getU16] couldn't get byte: reached end of buffer\n");
		return 0;
	}
	uint16_t value = *(uint16_t*) (_buffer + _position);
	if (!peek)
		_position += 2;
	return value;
}

uint32_t Message::getU32(bool peek) {
	if (!canRead(4)) {
		printf("[Message::getU32] couldn't get byte: reached end of buffer\n");
		return 0;
	}
	uint32_t value = *(uint32_t*) (_buffer + _position);
	if (!peek)
		_position += 4;
	return value;
}

uint64_t Message::getU64(bool peek) {
	if (!canRead(8)) {
		printf("[Message::getU64] couldn't get byte: reached end of buffer\n");
		return 0;
	}
	uint64_t value = *(uint64_t*) (_buffer + _position);
	if (!peek)
		_position += 8;
	return value;
}

std::string Message::getString() {
	uint16_t length = getU16();
	char* c_string[length];
	memcpy(c_string, _buffer + _position, length);
	_position += length;
	std::string value((const char*) c_string, length);
	return value;
}

Position Message::getPosition() {
	uint16_t x = getU16();
	uint16_t y = getU16();
	uint8_t z = getByte();
	return Position(x, y, z);
}
