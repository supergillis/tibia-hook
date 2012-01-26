#include "ScriptHandler.h"
#include "Packet.h"

#include <QDebug>
#include <QFile>

QScriptValue mySpecialQObjectConstructor(QScriptContext* context, QScriptEngine* engine) {
	return engine->newQObject(new Packet(), QScriptEngine::ScriptOwnership);
}

ScriptHandler::ScriptHandler(Hook* hook) :
		Handler(hook), _engine(this), _handler(_engine.newObject()) {
	QScriptValue hookValue = _engine.newQObject(this->hook(), QScriptEngine::QtOwnership);
	QScriptValue packetConstructor = _engine.newFunction(mySpecialQObjectConstructor);
	QScriptValue packetMetaObject = _engine.newQMetaObject(&Packet::staticMetaObject, packetConstructor);

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
		QScriptValue packet = _engine.newQObject(new Packet(), QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		handler.call(_handler, args << packet);
	}
	return false;
}

void ScriptHandler::handleIncomingMessage(const EncryptedMessage& message) {
	// Do nothing yet
}

bool ScriptHandler::handleIncomingMessageInternal(const EncryptedMessage& message) {
	return false;
}
