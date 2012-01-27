#include "ReadWritePacket.h"

#define max(a, b) a > b ? a : b

ReadWritePacket::ReadWritePacket() :
		_data(32, 0), _length(0) {
}

quint16 ReadWritePacket::length() const {
	return _length;
}

const quint8* ReadWritePacket::data() const {
	return (const quint8*) _data.data();
}

void ReadWritePacket::reserve(quint16 size) {
	quint16 newSize = _position + size;
	if (_data.length() < newSize) {
		_data.resize(newSize);
	}
}

void ReadWritePacket::writeU8(quint8 value) {
	reserve(1);
	*(_data.data() + _position) = value;
	_position += 1;
	_length = max(_length, _position);
}

void ReadWritePacket::writeU16(quint16 value) {
	reserve(2);
	*(quint16*) (_data.data() + _position) = value;
	_position += 2;
	_length = max(_length, _position);
}

void ReadWritePacket::writeU32(quint32 value) {
	reserve(4);
	*(quint16*) (_data.data() + _position) = value;
	_position += 4;
	_length = max(_length, _position);
}

void ReadWritePacket::writeU64(quint64 value) {
	reserve(8);
	*(quint16*) (_data.data() + _position) = value;
	_position += 8;
	_length = max(_length, _position);
}

void ReadWritePacket::writeString(const QString& value) {
	QByteArray array = value.toAscii();
	quint16 length = array.length();

	writeU16(length);
	reserve(length);
	memcpy(_data.data() + _position, array.constData(), length);
	_position += length;
	_length = max(_length, _position);
}
