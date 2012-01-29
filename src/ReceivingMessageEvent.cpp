#include "ReceivingMessageEvent.h"

ReceivingMessageEvent::ReceivingMessageEvent(const MessageType type, const EncryptedMessage* message) :
		QEvent(ReceivingMessageEvent::EventType), _type(type), _message(*message) {
}

const ReceivingMessageEvent::MessageType ReceivingMessageEvent::messageType() const {
	return _type;
}

const EncryptedMessage* ReceivingMessageEvent::message() const {
	return &_message;
}
