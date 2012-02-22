#include <QDebug>
#include <QDir>
#include <QScriptValueIterator>

#include "ScriptHandler.h"

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

bool ScriptHandler::receiveFromClient(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacket* packet = new ReadOnlyPacket(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
		return result.isBool() ? result.toBool() : false;
	}
	return false;
}

bool ScriptHandler::receiveFromServer(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacket* packet = new ReadOnlyPacket(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
		return result.isBool() ? result.toBool() : false;
	}
	return false;
}
