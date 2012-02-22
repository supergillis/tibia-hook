#include "ReadOnlyPacket.h"

ReadOnlyPacket::ReadOnlyPacket(const QByteArray& data) :
		Packet(), raw_(data) {
}

ReadOnlyPacket::ReadOnlyPacket(const quint8* buffer, quint16 length) :
		Packet(), raw_((const char*) buffer, length) {
}

quint16 ReadOnlyPacket::length() const {
	return raw_.length();
}

const quint8* ReadOnlyPacket::data() const {
	return (const quint8*) raw_.data();
}
