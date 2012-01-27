#include "Packet.h"

Packet::Packet() :
		QObject(), _position(0) {
}

quint16 Packet::position() const {
	return _position;
}

void Packet::setPosition(quint16 position) {
	_position = position;
}

void Packet::skip(quint16 count) {
	_position += count;
}

bool Packet::has(quint16 count) const {
	return length() - _position >= count;
}

quint8 Packet::readU8() {
	if (has(1)) {
		quint8 value = *(data() + _position);
		_position += 1;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint16 Packet::readU16() {
	if (has(2)) {
		quint16 value = *(quint16*) (data() + _position);
		_position += 2;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint32 Packet::readU32() {
	if (has(2)) {
		quint32 value = *(quint32*) (data() + _position);
		_position += 4;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint64 Packet::readU64() {
	if (has(2)) {
		quint64 value = *(quint64*) (data() + _position);
		_position += 8;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

QString Packet::readString() {
	quint16 length = readU16();
	if (has(length)) {
		QString value = QString::fromAscii((const char*) (data() + _position), length);
		_position += length;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return QString();
}
