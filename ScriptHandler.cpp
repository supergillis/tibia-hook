#include "ScriptHandler.h"
#include "Packet.h"

#include <QDebug>
#include <QFile>

ScriptHandler::ScriptHandler(Hook* hook) :
		Handler(hook), _engine(this), _engineAgent(&_engine), _handler(_engine.newObject()) {
	QScriptValue hookValue = _engine.newQObject(this->hook(), QScriptEngine::QtOwnership);
	QScriptValue packetConstructor = _engine.newFunction(ScriptHandler::packetConstructor);
	QScriptValue packetMetaObject = _engine.newQMetaObject(&Packet::staticMetaObject, packetConstructor);

	_engine.setAgent(&_engineAgent);
	_engine.globalObject().setProperty("Hook", hookValue);
	_engine.globalObject().setProperty("Packet", packetMetaObject);
	_engine.globalObject().setProperty("Handler", _handler);

	QFile script("/home/gillis/projects/tibia-hook-linux/main.js");
	if (script.open(QFile::ReadOnly)) {
		_engine.evaluate(script.readAll());
	}
}

void ScriptHandler::handleOutgoingMessage(const EncryptedMessage& message) {
	if (!handleOutgoingMessageInternal(message)) {
		hook()->write(message);
	}
}

bool ScriptHandler::handleOutgoingMessageInternal(const EncryptedMessage& message) {
	QScriptValue handler = _handler.property("handleOutgoingPacket");
	if (handler.isFunction()) {
		DecryptedMessage decrypted = DecryptedMessage::decrypt(message, Encryption::XTEA::TIBIA_KEY);
		if (decrypted.isValid()) {
			QScriptValue packet = _engine.newQObject(new Packet(decrypted), QScriptEngine::ScriptOwnership);
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
	return engine->newQObject(new Packet(), QScriptEngine::ScriptOwnership);
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
