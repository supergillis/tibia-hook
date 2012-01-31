#include "ReadOnlyPacket.h"

ReadOnlyPacket::ReadOnlyPacket(const DecryptedMessage* message) :
		Packet(), message_(*message) {
}

const DecryptedMessage* ReadOnlyPacket::message() const {
	return &message_;
}

quint16 ReadOnlyPacket::length() const {
	return message_.length();
}

const quint8* ReadOnlyPacket::data() const {
	return message_.data();
}
