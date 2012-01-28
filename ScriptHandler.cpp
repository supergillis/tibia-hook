#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "ReadWritePacket.h"
#include "ReadOnlyPacket.h"
#include "Hook.h"
#include "Memory.h"

#include <QScriptContextInfo>
#include <QDebug>

ScriptHandler::ScriptHandler() :
		Handler(), _engine(this), _handlerObject(_engine.newObject()) {
	QScriptValue packetObject = _engine.newQMetaObject(&ReadWritePacket::staticMetaObject, _engine.newFunction(Handlers::Packet::constructor));

	QScriptValue hookObject(_engine.newObject());
	hookObject.setProperty("write", _engine.newFunction(Handlers::Hook::write));
	hookObject.setProperty("sendKeyPress", _engine.newFunction(Handlers::Hook::sendKeyPress));

	QScriptValue memoryObject(_engine.newObject());
	memoryObject.setProperty("readU8", _engine.newFunction(Handlers::Memory::readU8));
	memoryObject.setProperty("readU16", _engine.newFunction(Handlers::Memory::readU16));
	memoryObject.setProperty("readU32", _engine.newFunction(Handlers::Memory::readU32));
	memoryObject.setProperty("readString", _engine.newFunction(Handlers::Memory::readString));

	QScriptValue environmentObject(_engine.newObject());
	QScriptValue reloadFunction = _engine.newFunction(Handlers::Environment::reload);
	reloadFunction.setData(_engine.newQObject(this));

	environmentObject.setProperty("reload", reloadFunction);
	environmentObject.setProperty("require", _engine.newFunction(Handlers::Environment::require));

	_engine.globalObject().setProperty("Hook", hookObject);
	_engine.globalObject().setProperty("Memory", memoryObject);
	_engine.globalObject().setProperty("Packet", packetObject);
	_engine.globalObject().setProperty("Environment", environmentObject);
	_engine.globalObject().setProperty("Handler", _handlerObject);

	reload();
}

void ScriptHandler::reload() {
	_engine.pushContext();
	_engine.clearRequiredFiles();
	_engine.require("main.js");
	_engine.popContext();
}

void ScriptHandler::handleOutgoingMessage(const EncryptedMessage* message) {
	if (!handleOutgoingMessageInternal(message)) {
		Hook::getInstance()->write(message);
	}
}

bool ScriptHandler::handleOutgoingMessageInternal(const EncryptedMessage* message) {
	QScriptValue handler = _handlerObject.property("handleOutgoingPacket");
	if (handler.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packet = _engine.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership);
			QScriptValueList args;
			QScriptValue result = handler.call(_handlerObject, args << packet);
			return result.isBool() ? result.toBool() : false;
		}
	}
	return false;
}

void ScriptHandler::handleIncomingMessage(const EncryptedMessage* message) {
	// Do nothing yet
}

bool ScriptHandler::handleIncomingMessageInternal(const EncryptedMessage* message) {
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

QScriptValue Handlers::Hook::write(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			::Hook::getInstance()->write(&message);
			return QScriptValue(true);
		}
	}
	return context->throwError("write(Packet) only accepts one argument");
}

QScriptValue Handlers::Hook::sendKeyPress(QScriptContext* context, QScriptEngine* engine) {
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
