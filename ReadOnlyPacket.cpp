#include "ReadOnlyPacket.h"

ReadOnlyPacket::ReadOnlyPacket(const DecryptedMessage& message) :
		Packet(), _message(message) {
}

quint16 ReadOnlyPacket::length() const {
	return _message.length();
}

const quint8* ReadOnlyPacket::data() const {
	return _message.data();
}
