#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "ReadWritePacket.h"
#include "ReadOnlyPacket.h"
#include "Hook.h"

#include <QDebug>
#include <QDir>
#include <QFile>

ScriptHandler::ScriptHandler() :
		Handler(), _engine(this), _handler(_engine.newObject()), _hook(_engine.newObject()) {
	QScriptValue packetConstructor = _engine.newFunction(ScriptHandler::packetConstructor);
	QScriptValue packetMetaObject = _engine.newQMetaObject(&Packet::staticMetaObject, packetConstructor);

	_hook.setProperty("write", _engine.newFunction(ScriptHandler::hookWrite));

	_engine.globalObject().setProperty("require", _engine.newFunction(ScriptHandler::require));
	_engine.globalObject().setProperty("Hook", _hook);
	_engine.globalObject().setProperty("Packet", packetMetaObject);
	_engine.globalObject().setProperty("Handler", _handler);

	QDir current = QDir::current();
	QString main = current.absoluteFilePath("main.js");

	_engine.require(main);
}

void ScriptHandler::handleOutgoingMessage(const EncryptedMessage& message) {
	if (!handleOutgoingMessageInternal(message)) {
		Hook::getInstance()->write(message);
	}
}

bool ScriptHandler::handleOutgoingMessageInternal(const EncryptedMessage& message) {
	QScriptValue handler = _handler.property("handleOutgoingPacket");
	if (handler.isFunction()) {
		DecryptedMessage decrypted(&message);
		if (decrypted.isValid()) {
			QScriptValue packet = _engine.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership);
			QScriptValueList args;
			QScriptValue result = handler.call(_handler, args << packet);
			return result.isBool() ? result.toBool() : false;
		}
	}
	return false;
}

void ScriptHandler::handleIncomingMessage(const EncryptedMessage& message) {
	// Do nothing yet
}

bool ScriptHandler::handleIncomingMessageInternal(const EncryptedMessage& message) {
	return false;
}

QScriptValue ScriptHandler::require(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ScriptEngine* scriptEngine = qobject_cast<ScriptEngine*>(engine);
		if (value.isString() && scriptEngine) {
			QString required = value.toString();
			return scriptEngine->require(required);
		}
	}
	return context->throwError("import(String) only accepts one argument");
}

QScriptValue ScriptHandler::packetConstructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		return engine->newQObject(new ReadWritePacket(), QScriptEngine::ScriptOwnership);
	}
	return context->throwError("Packet() only accepts no arguments");
}

QScriptValue ScriptHandler::hookWrite(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			Hook::getInstance()->write(message);
			return QScriptValue(true);
		}
	}
	return context->throwError("write(Packet) only accepts one argument");
}
