#include "Packet.h"

Packet::Packet() :
		QObject() {
}

Packet::Packet(const DecryptedMessage& message) :
		QObject() {
	_data = QByteArray((const char*) message.data(), message.length());
}

quint8 Packet::readU8() {
	quint8 value = *(_data.constData() + _position);
	_position += 1;
	return value;
}

quint16 Packet::readU16() {
	quint16 value = *(quint16*) (_data.constData() + _position);
	_position += 2;
	return value;
}

quint32 Packet::readU32() {
	quint32 value = *(quint32*) (_data.constData() + _position);
	_position += 4;
	return value;
}

quint64 Packet::readU64() {
	quint64 value = *(quint64*) (_data.constData() + _position);
	_position += 8;
	return value;
}

QString Packet::readString() {
	quint16 length = readU16();
	QString value = QString::fromLocal8Bit(_data.data(), length);
	_position += length;
	return value;
}
