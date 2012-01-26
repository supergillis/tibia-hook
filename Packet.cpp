#include "Packet.h"

#include <QDebug>

Packet::Packet() :
		QObject() {
}

Packet::Packet(const DecryptedMessage& message) :
		QObject(), _data((const char*) message.data(), message.length()), _length(message.length()), _position(0) {
}

bool Packet::has(quint16 count) {
	return _length - _position >= count;
}

quint8 Packet::readU8() {
	if (has(1)) {
		quint8 value = *(_data.constData() + _position);
		_position += 1;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint16 Packet::readU16() {
	if (has(2)) {
		quint16 value = *(quint16*) (_data.constData() + _position);
		_position += 2;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint32 Packet::readU32() {
	if (has(2)) {
		quint32 value = *(quint32*) (_data.constData() + _position);
		_position += 4;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint64 Packet::readU64() {
	if (has(2)) {
		quint64 value = *(quint64*) (_data.constData() + _position);
		_position += 8;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

QString Packet::readString() {
	quint16 length = readU16();
	if (has(length)) {
		QString value = QString::fromAscii(_data.constData() + _position, length);
		_position += length;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return QString();
}
