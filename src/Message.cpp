#include "Message.h"

Message::Message() {
}

void Message::initialize(const quint8* buffer, quint16 length) {
	raw_ = QByteArray((const char*) buffer, length);
}

quint16 Message::rawLength() const {
	return raw_.length();
}

const quint8* Message::rawData() const {
	return (const quint8*) raw_.constData();
}
