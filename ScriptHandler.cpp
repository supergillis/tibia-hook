#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "ReadWritePacket.h"
#include "ReadOnlyPacket.h"
#include "Hook.h"

#include <QDebug>
#include <QFile>

ScriptHandler::ScriptHandler() :
		Handler(), _engine(this), _engineAgent(&_engine), _handler(_engine.newObject()), _hook(_engine.newObject()) {
	QScriptValue packetConstructor = _engine.newFunction(ScriptHandler::packetConstructor);
	QScriptValue packetMetaObject = _engine.newQMetaObject(&Packet::staticMetaObject, packetConstructor);

	_hook.setProperty("write", _engine.newFunction(ScriptHandler::hookWrite));

	_engine.setAgent(&_engineAgent);
	_engine.globalObject().setProperty("Hook", _hook);
	_engine.globalObject().setProperty("Packet", packetMetaObject);
	_engine.globalObject().setProperty("Handler", _handler);

	QFile script("/home/gillis/projects/tibia-hook/main.js");
	if (script.open(QFile::ReadOnly)) {
		_engine.evaluate(script.readAll());
	}
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

QScriptValue ScriptHandler::packetConstructor(QScriptContext* context, QScriptEngine* engine) {
	return engine->newQObject(new ReadWritePacket(), QScriptEngine::ScriptOwnership);
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
	return context->throwError("write accepts only one argument");
}

ScriptEngineAgent::ScriptEngineAgent(QScriptEngine* engine) :
		QScriptEngineAgent(engine) {
}

void ScriptEngineAgent::exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler) {
	if (!hasHandler) {
		qWarning() << "unhandled exception";
		qWarning() << exception.toString().toAscii();
	}
}
