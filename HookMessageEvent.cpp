#include "HookMessageEvent.h"

HookMessageEvent::HookMessageEvent(const Type type, const EncryptedMessage* message) :
		QEvent(HookMessageEvent::EventType), _type(type), _message(*message) {
}

const HookMessageEvent::Type HookMessageEvent::type() const {
	return _type;
}

const EncryptedMessage* HookMessageEvent::message() const {
	return &_message;
}
