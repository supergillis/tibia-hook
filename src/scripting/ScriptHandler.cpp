#include <QDebug>
#include <QDir>
#include <QScriptValueIterator>

#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "Packet.h"
#include "ReadOnlyPacket.h"
#include "Hook.h"

#include "ClassModule.h"
#include "EnvironmentModule.h"
#include "PacketModule.h"

ScriptHandler::ScriptHandler(Hook* hook) :
		Handler(hook), hook_(hook), engine_(this), ModuleManager(hook, &engine_) {
	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");
}

Hook* ScriptHandler::hook() {
	return hook_;
}

QScriptEngine* ScriptHandler::engine() {
	return &engine_;
}

void ScriptHandler::reload() {
	EnvironmentModule* environmentModule = (EnvironmentModule*) lookup(EnvironmentModule::PLUGIN_NAME);
	if (environmentModule) {
		environmentModule->reload();
	}
}

void ScriptHandler::receiveFromClient(const EncryptedMessage* message) {
	if (!receiveFromClientInternal(message)) {
		hook_->sendToServer(message);
	}
}

bool ScriptHandler::receiveFromClientInternal(const EncryptedMessage* message) {
	QScriptValue callback = engine_.globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packet = engine_.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership);
			QScriptValueList args;
			QScriptValue result = callback.call(engine_.globalObject(), args << packet);
			return result.isBool() ? result.toBool() : false;
		}
	}
	return false;
}

void ScriptHandler::receiveFromServer(const EncryptedMessage* message) {
	QScriptValue callback = engine_.globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packet = engine_.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership);
			QScriptValueList args;
			QScriptValue result = callback.call(engine_.globalObject(), args << packet);
		}
	}
}
