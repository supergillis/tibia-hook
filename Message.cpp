#include "Message.h"

Message::Message() {
}

Message::Message(const uint8_t* buffer, uint16_t length) {
	_raw = QByteArray((const char*) buffer, length);
}

uint16_t Message::rawLength() const {
	return _raw.length();
}

const uint8_t* Message::rawData() const {
	return (const uint8_t*) _raw.constData();
}
