#include "ReceivingMessageEvent.h"

ReceivingMessageEvent::ReceivingMessageEvent(const MessageType type, const EncryptedMessage* message) :
		QEvent(ReceivingMessageEvent::EventType), type_(type), message_(*message) {
}

const ReceivingMessageEvent::MessageType ReceivingMessageEvent::messageType() const {
	return type_;
}

const EncryptedMessage* ReceivingMessageEvent::message() const {
	return &message_;
}
