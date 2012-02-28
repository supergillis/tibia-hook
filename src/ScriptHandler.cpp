#include "ScriptHandler.h"
#include "ReadOnlyPacketProxy.h"

ScriptHandler::ScriptHandler(Hook* hook) :
		Handler(), engine_(hook) {
	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");
}

ScriptHandler::~ScriptHandler() {
	foreach(ScriptPluginInterface* plugin, plugins_) {
		qDebug() << "uninstalling" << plugin->name();
		plugin->uninstall();
	}
}

void ScriptHandler::reload() {
	engine_.reload();
}

void ScriptHandler::install(ScriptPluginInterface* plugin) {
	qDebug() << "installing" << plugin->name();
	plugin->install(&engine_);
	plugins_.append(plugin);
}

bool ScriptHandler::receiveFromClient(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
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
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		callback.call(engine_.globalObject(), args << value);
	}
}
