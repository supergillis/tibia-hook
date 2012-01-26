#include "Packet.h"

Packet::Packet() {
}

uint8_t Packet::readU8() {
	uint8_t value = *_data.constData();
	_position += 1;
	return value;
}

uint16_t Packet::readU16() {
	uint16_t value = *(uint16_t*) (_data.constData() + _position);
	_position += 2;
	return value;
}

uint32_t Packet::readU32() {
	uint32_t value = *(uint32_t*) (_data.constData() + _position);
	_position += 4;
	return value;
}

uint64_t Packet::readU64() {
	uint64_t value = *(uint64_t*) (_data.constData() + _position);
	_position += 8;
	return value;
}

QString Packet::readString() {
	uint16_t length = readU16();
	QString value = QString::fromLocal8Bit(_data.data(), length);
	_position += length;
	return value;
}
