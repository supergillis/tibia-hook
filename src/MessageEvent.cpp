#include "MessageEvent.h"

MessageEvent::MessageEvent(const MessageType type, const EncryptedMessage* message) :
		QEvent(MessageEvent::EventType), type_(type), message_(*message) {
}

const MessageEvent::MessageType MessageEvent::messageType() const {
	return type_;
}

const EncryptedMessage* MessageEvent::message() const {
	return &message_;
}
