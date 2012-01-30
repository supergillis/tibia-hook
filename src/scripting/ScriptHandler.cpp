#include <QDebug>
#include <QDir>
#include <QScriptValueIterator>

#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "Packet.h"
#include "Hook.h"

#include "DebuggerModule.h"
#include "ClassModule.h"
#include "ClientModule.h"
#include "EnvironmentModule.h"
#include "MemoryModule.h"
#include "NetworkModule.h"
#include "PacketModule.h"

ScriptHandler::ScriptHandler(QObject* parent) :
		Handler(parent), engine_(this) {
	install(new DebuggerModule());
	install(new ClassModule());
	install(new ClientModule());
	install(new EnvironmentModule());
	install(new MemoryModule());
	install(new NetworkModule());
	install(new PacketModule());

	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");
}

QScriptEngine* ScriptHandler::scriptEngine() {
	return &engine_;
}

const QScriptEngine* ScriptHandler::scriptEngine() const {
	return &engine_;
}

bool ScriptHandler::install(Module* module) {
	if (module->install(this)) {
		modules_.insert(module->name(), module);
		return true;
	}
	return false;
}

Module* ScriptHandler::lookup(const QString& name) {
	return modules_.value(name, NULL);
}

void ScriptHandler::reload() {
	engine_.pushContext();
	EnvironmentModule::setRequiredFiles(&engine_, QStringList());
	EnvironmentModule::require(&engine_, "Main.js");
	engine_.popContext();
}

void ScriptHandler::receiveFromClient(const EncryptedMessage* message) {
	if (!receiveFromClientInternal(message)) {
		Hook::getInstance()->sendToServer(message);
	}
}

bool ScriptHandler::receiveFromClientInternal(const EncryptedMessage* message) {
	QScriptValue callback = engine_.globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packetClass = engine_.globalObject().property("Packet");
			QScriptValue packet = ClassModule::createInstance(&engine_, packetClass);
			packet.setData(engine_.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership));
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
			QScriptValue packetClass = engine_.globalObject().property("Packet");
			QScriptValue packet = ClassModule::createInstance(&engine_, packetClass);
			packet.setData(engine_.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership));
			QScriptValueList args;
			QScriptValue result = callback.call(engine_.globalObject(), args << packet);
		}
	}
}
