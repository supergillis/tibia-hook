#include "ScriptHandler.h"
#include "ReadOnlyPacket.h"
#include "EnvironmentModule.h"

ScriptHandler::ScriptHandler(Hook* hook) :
		QObject(hook), Handler(), hook_(hook), engine_(this), moduleManager_(new ModuleManager(hook, &engine_)) {
	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");
}

ScriptHandler::~ScriptHandler() {
	delete moduleManager_;
}

Hook* ScriptHandler::hook() {
	return hook_;
}

QScriptEngine* ScriptHandler::engine() {
	return &engine_;
}

void ScriptHandler::install(Module* module) {
	moduleManager_->install(module);
}

void ScriptHandler::reload() {
	EnvironmentModule* environmentModule = (EnvironmentModule*) moduleManager_->lookup(EnvironmentModule::PLUGIN_NAME);
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
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}

void ScriptHandler::receiveFromServer(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacket* packet = new ReadOnlyPacket(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
	}
}
