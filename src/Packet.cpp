#include "Packet.h"

Packet::Packet() :
		QObject(), position_(0) {
}

quint16 Packet::position() const {
	return position_;
}

void Packet::setPosition(quint16 position) {
	position_ = position;
}

void Packet::skip(quint16 count) {
	position_ += count;
}

bool Packet::has(quint16 count) const {
	return length() - position_ >= count;
}

quint8 Packet::readU8() {
	if (has(1)) {
		quint8 value = *(data() + position_);
		position_ += 1;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint16 Packet::readU16() {
	if (has(2)) {
		quint16 value = *(quint16*) (data() + position_);
		position_ += 2;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint32 Packet::readU32() {
	if (has(2)) {
		quint32 value = *(quint32*) (data() + position_);
		position_ += 4;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

quint64 Packet::readU64() {
	if (has(2)) {
		quint64 value = *(quint64*) (data() + position_);
		position_ += 8;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return 0;
}

QString Packet::readString() {
	quint16 length = readU16();
	if (has(length)) {
		QString value = QString::fromAscii((const char*) (data() + position_), length);
		position_ += length;
		return value;
	}
	qWarning() << PACKET_END_OF_FILE;
	return QString();
}
