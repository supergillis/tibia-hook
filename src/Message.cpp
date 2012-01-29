#include "Message.h"

Message::Message() {
}

void Message::initialize(const quint8* buffer, quint16 length) {
	_raw = QByteArray((const char*) buffer, length);
}

quint16 Message::rawLength() const {
	return _raw.length();
}

const quint8* Message::rawData() const {
	return (const quint8*) _raw.constData();
}
