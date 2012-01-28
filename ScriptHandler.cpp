#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "ReadWritePacket.h"
#include "ReadOnlyPacket.h"
#include "Hook.h"
#include "Memory.h"

#include <QDebug>

ScriptHandler::ScriptHandler() :
		Handler(), _engine(this) {
	QScriptValue packetObject = _engine.newQMetaObject(&ReadWritePacket::staticMetaObject, _engine.newFunction(Handlers::Packet::constructor));

	_networkObject = _engine.newObject();
	_networkObject.setProperty("sendToServer", _engine.newFunction(Handlers::Network::sendToServer));
	_networkObject.setProperty("sendToClient", _engine.newFunction(Handlers::Network::sendToClient));

	QScriptValue clientObject(_engine.newObject());
	clientObject.setProperty("sendPacket", _engine.newFunction(Handlers::Client::sendPacket));
	clientObject.setProperty("sendKeyPress", _engine.newFunction(Handlers::Client::sendKeyPress));

	QScriptValue memoryObject(_engine.newObject());
	memoryObject.setProperty("readU8", _engine.newFunction(Handlers::Memory::readU8));
	memoryObject.setProperty("readU16", _engine.newFunction(Handlers::Memory::readU16));
	memoryObject.setProperty("readU32", _engine.newFunction(Handlers::Memory::readU32));
	memoryObject.setProperty("readString", _engine.newFunction(Handlers::Memory::readString));

	QScriptValue reloadFunction = _engine.newFunction(Handlers::Environment::reload);
	reloadFunction.setData(_engine.newQObject(this));

	QScriptValue environmentObject(_engine.newObject());
	environmentObject.setProperty("reload", reloadFunction);
	environmentObject.setProperty("require", _engine.newFunction(Handlers::Environment::require));

	_engine.globalObject().setProperty("Network", _networkObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	_engine.globalObject().setProperty("Client", clientObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	_engine.globalObject().setProperty("Memory", memoryObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	_engine.globalObject().setProperty("Packet", packetObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	_engine.globalObject().setProperty("Environment", environmentObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);

	reload();
}

void ScriptHandler::reload() {
	_engine.pushContext();
	_engine.clearRequiredFiles();
	_engine.require("main.js");
	_engine.popContext();
}

void ScriptHandler::receiveFromClient(const EncryptedMessage* message) {
	if (!receiveFromClientInternal(message)) {
		Hook::getInstance()->sendToServer(message);
	}
}

bool ScriptHandler::receiveFromClientInternal(const EncryptedMessage* message) {
	QScriptValue callback = _networkObject.property("receiveFromClient");
	if (callback.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packet = _engine.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership);
			QScriptValueList args;
			QScriptValue result = callback.call(_networkObject, args << packet);
			return result.isBool() ? result.toBool() : false;
		}
	}
	return false;
}

void ScriptHandler::receiveFromServer(const EncryptedMessage* message) {
}

bool ScriptHandler::receiveFromServerInternal(const EncryptedMessage* message) {
	QScriptValue callback = _networkObject.property("receiveFromServer");
	if (callback.isFunction()) {
		qDebug() << "received message from server";
	}
	return false;
}

static QScriptValue Handlers::Network::sendToServer(QScriptContext*, QScriptEngine*) {
	return false;
}

static QScriptValue Handlers::Network::sendToClient(QScriptContext*, QScriptEngine*) {
	return false;
}

QScriptValue Handlers::Environment::reload(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue value = context->callee().data();
		ScriptHandler* handler = qobject_cast<ScriptHandler*>(value.toQObject());
		if (handler) {
			handler->reload();
			return QScriptValue(true);
		}
	}
	return context->throwError("reload() accepts no arguments");
}

QScriptValue Handlers::Environment::require(QScriptContext* context, QScriptEngine* engine) {
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

QScriptValue Handlers::Packet::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		return engine->newQObject(new ReadWritePacket(), QScriptEngine::ScriptOwnership);
	}
	return context->throwError("Packet() only accepts no arguments");
}

QScriptValue Handlers::Client::sendPacket(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			::Hook::getInstance()->sendToServer(&message);
			return QScriptValue(true);
		}
	}
	return context->throwError("write(Packet) only accepts one argument");
}

QScriptValue Handlers::Client::sendKeyPress(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			::Hook::getInstance()->sendKeyPress(value.toInt32());
			return true;
		}
		else {
			qWarning() << "not a number";
		}
	}
	qWarning() << "wrong usage of sendKeyPress(Number)";
	return false;
}

QScriptValue Handlers::Memory::readU8(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readU8(address.toUInt32());
		}
	}
	return context->throwError("readU8(Address) only accepts one argument");
}

QScriptValue Handlers::Memory::readU16(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readU16(address.toUInt32());
		}
	}
	return context->throwError("readU16(Address) only accepts one argument");
}

QScriptValue Handlers::Memory::readU32(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readU32(address.toUInt32());
		}
	}
	return context->throwError("readU32(Address) only accepts one argument");
}

QScriptValue Handlers::Memory::readString(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readString(address.toUInt32());
		}
	}
	return context->throwError("readU32(Address) only accepts one argument");
}
