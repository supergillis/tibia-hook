#include <QDebug>
#include <QDir>
#include <QFile>

#include "ScriptHandler.h"
#include "DecryptedMessage.h"
#include "Packet.h"
#include "ReadWritePacket.h"
#include "ReadOnlyPacket.h"
#include "Memory.h"
#include "Hook.h"

ScriptHandler::ScriptHandler(QObject* parent) :
		Handler(parent), _engine(this) {
	_instanceHandle = _engine.toStringHandle("instance");
	_constructorHandle = _engine.toStringHandle("constructor");
	_extendHandle = _engine.toStringHandle("extend");
	_createHandle = _engine.toStringHandle("create");
	_extendedHandle = _engine.toStringHandle("extended");

	_receiveFromClientHandle = _engine.toStringHandle("receiveFromClient");
	_receiveFromServerHandle = _engine.toStringHandle("receiveFromServer");

	_classObject = createClass(_engine.newArray(0));

	_engine.globalObject().setProperty("Class", _classObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);

	initializePacketObject();
	initializeClientObject();
	initializeMemoryObject();
	initializeNetworkObject();
}

QScriptEngine* ScriptHandler::scriptEngine() {
	return &_engine;
}

const QScriptEngine* ScriptHandler::scriptEngine() const {
	return &_engine;
}

QScriptValue ScriptHandler::classObject() {
	return _classObject;
}

const QScriptValue ScriptHandler::classObject() const {
	return _classObject;
}

void ScriptHandler::install(Module* module) {
	module->install(this);
}

void ScriptHandler::reload() {
	_engine.pushContext();
	_requiredFiles.clear();
	require("Main.js");
	_engine.popContext();
}

void ScriptHandler::require(const QString& path) {
	if (!_requiredFiles.contains(path)) {
		QDir scripts(QDir::current());
		if (!scripts.cd("scripts")) {
			qWarning() << scripts.absolutePath() << "does not exist";
			return;
		}

		QFile file(scripts.absoluteFilePath(path));
		if (!file.open(QFile::ReadOnly)) {
			qWarning() << file.fileName() << "does not exist";
			return;
		}

		_engine.evaluate(file.readAll(), path);
		_requiredFiles << path;
	}
}

void ScriptHandler::initializeClientObject() {
	QScriptValue clientObject = createInstance(_classObject, _engine.newArray(0));
	clientObject.setProperty("sendPacket", _engine.newFunction(Handlers::Client::sendPacket));
	clientObject.setProperty("sendKeyPress", _engine.newFunction(Handlers::Client::sendKeyPress));

	_engine.globalObject().setProperty("Client", clientObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void ScriptHandler::initializeMemoryObject() {
	QScriptValue memoryObject = createInstance(_classObject, _engine.newArray(0));
	memoryObject.setProperty("readU8", _engine.newFunction(Handlers::Memory::readU8));
	memoryObject.setProperty("readU16", _engine.newFunction(Handlers::Memory::readU16));
	memoryObject.setProperty("readU32", _engine.newFunction(Handlers::Memory::readU32));
	memoryObject.setProperty("readString", _engine.newFunction(Handlers::Memory::readString));

	_engine.globalObject().setProperty("Memory", memoryObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void ScriptHandler::initializeNetworkObject() {
	_networkObject = createInstance(_classObject, _engine.newArray(0));
	_networkObject.setProperty("sendToServer", _engine.newFunction(Handlers::Network::sendToServer));
	_networkObject.setProperty("sendToClient", _engine.newFunction(Handlers::Network::sendToClient));

	_engine.globalObject().setProperty("Network", _networkObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void ScriptHandler::initializePacketObject() {
	_packetObject = createClass(_classObject, _engine.newArray(0));

	QScriptValue packetInstance = _packetObject.property(_instanceHandle);
	packetInstance.setProperty(_constructorHandle, _engine.newFunction(Handlers::PacketWrite::constructor));
	packetInstance.setProperty("readU8", _engine.newFunction(Handlers::PacketRead::readU8));
	packetInstance.setProperty("readU16", _engine.newFunction(Handlers::PacketRead::readU16));
	packetInstance.setProperty("readU32", _engine.newFunction(Handlers::PacketRead::readU32));
	packetInstance.setProperty("readString", _engine.newFunction(Handlers::PacketRead::readString));
	packetInstance.setProperty("writeU8", _engine.newFunction(Handlers::PacketWrite::writeU8));
	packetInstance.setProperty("writeU16", _engine.newFunction(Handlers::PacketWrite::writeU16));
	packetInstance.setProperty("writeU32", _engine.newFunction(Handlers::PacketWrite::writeU32));
	packetInstance.setProperty("writeString", _engine.newFunction(Handlers::PacketWrite::writeString));

	_engine.globalObject().setProperty("Packet", _packetObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void ScriptHandler::receiveFromClient(const EncryptedMessage* message) {
	if (!receiveFromClientInternal(message)) {
		Hook::getInstance()->sendToServer(message);
	}
}

bool ScriptHandler::receiveFromClientInternal(const EncryptedMessage* message) {
	QScriptValue callback = _networkObject.property(_receiveFromClientHandle);
	if (callback.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packet = createInstance(_packetObject, _engine.newArray(0));
			packet.setData(_engine.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership));
			QScriptValueList args;
			QScriptValue result = callback.call(_networkObject, args << packet);
			return result.isBool() ? result.toBool() : false;
		}
	}
	return false;
}

void ScriptHandler::receiveFromServer(const EncryptedMessage* message) {
	QScriptValue callback = _networkObject.property(_receiveFromServerHandle);
	if (callback.isFunction()) {
		DecryptedMessage decrypted(message);
		if (decrypted.isValid()) {
			QScriptValue packet = createInstance(_packetObject, _engine.newArray(0));
			packet.setData(_engine.newQObject(new ReadOnlyPacket(decrypted), QScriptEngine::ScriptOwnership));
			QScriptValueList args;
			QScriptValue result = callback.call(_networkObject, args << packet);
		}
	}
}

static QScriptValue Handlers::Network::sendToServer(QScriptContext* context, QScriptEngine*) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			Hook::getInstance()->sendToServer(&message);
			return true;
		}
	}
	return context->throwError("invalid call to sendToServer(Packet)");
}

static QScriptValue Handlers::Network::sendToClient(QScriptContext* context, QScriptEngine*) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			Hook::getInstance()->sendToClient(&message);
			return true;
		}
	}
	return context->throwError("invalid call to sendToServer(Packet)");
}

QScriptValue Handlers::Client::sendPacket(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			Hook::getInstance()->sendToClient(&message);
			return true;
		}
	}
	return context->throwError("invalid call to sendPacket(Packet)");
}

QScriptValue Handlers::Client::sendKeyPress(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			Hook::getInstance()->sendKeyPress(value.toInt32());
			return true;
		}
	}
	return context->throwError("invalid call to sendKeyPress(Number)");
}

QScriptValue Handlers::PacketRead::readU8(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readU8();
		}
	}
	return context->throwError("invalid call to readU8()");
}

QScriptValue Handlers::PacketRead::readU16(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readU16();
		}
	}
	return context->throwError("invalid call to readU16()");
}

QScriptValue Handlers::PacketRead::readU32(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readU32();
		}
	}
	return context->throwError("invalid call to readU32()");
}

QScriptValue Handlers::PacketRead::readString(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readString();
		}
	}
	return context->throwError("invalid call to readString()");
}

QScriptValue Handlers::PacketWrite::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue data = engine->newQObject(new ReadWritePacket(), QScriptEngine::ScriptOwnership);
		context->thisObject().setData(data);
		return true;
	}
	return context->throwError("invalid call to Packet constructor");
}

QScriptValue Handlers::PacketWrite::writeU8(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(context->thisObject().data().toQObject());
			if (packet) {
				packet->writeU8(value.toUInt32());
				return true;
			}
		}
	}
	return context->throwError("invalid call to writeU8(Number)");
}

QScriptValue Handlers::PacketWrite::writeU16(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(context->thisObject().data().toQObject());
			if (packet) {
				packet->writeU16(value.toUInt32());
				return true;
			}
		}
	}
	return context->throwError("invalid call to writeU16(Number)");
}

QScriptValue Handlers::PacketWrite::writeU32(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(context->thisObject().data().toQObject());
			if (packet) {
				packet->writeU32(value.toUInt32());
				return true;
			}
		}
	}
	return context->throwError("invalid call to writeU32(Number)");
}

QScriptValue Handlers::PacketWrite::writeString(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue value = context->argument(0);
		if (value.isString()) {
			ReadWritePacket* packet = qobject_cast<ReadWritePacket*>(context->thisObject().data().toQObject());
			if (packet) {
				packet->writeString(value.toString());
				return true;
			}
		}
	}
	return context->throwError("invalid call to writeString(Number)");
}

QScriptValue Handlers::Memory::readU8(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readU8(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readU8(Number)");
}

QScriptValue Handlers::Memory::readU16(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readU16(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readU16(Number)");
}

QScriptValue Handlers::Memory::readU32(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readU32(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readU32(Number)");
}

QScriptValue Handlers::Memory::readString(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return ::Memory::readString(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readString(Number)");
}

QScriptValue Handlers::Class::extend(QScriptContext* context, QScriptEngine* engine) {
	ScriptHandler* handler = qobject_cast<ScriptHandler*>(engine->parent());
	if (handler) {
		QScriptValue parent = context->thisObject();
		return handler->createClass(parent, context->argumentsObject());
	}
	return false;
}

QScriptValue Handlers::Class::create(QScriptContext* context, QScriptEngine* engine) {
	ScriptHandler* handler = qobject_cast<ScriptHandler*>(engine->parent());
	if (handler) {
		QScriptValue parent = context->thisObject();
		return handler->createInstance(parent, context->argumentsObject());
	}
	return false;
}

QScriptValue Handlers::Class::extended(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}

QScriptValue Handlers::Class::constructor(QScriptContext*, QScriptEngine*) {
	return QScriptValue::UndefinedValue;
}

QScriptValue ScriptHandler::createClassPrototype() {
	QScriptValue global = _engine.globalObject();
	QScriptValue prototype = _engine.newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty(_instanceHandle, createInstancePrototype(), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(_extendHandle, _engine.newFunction(Handlers::Class::extend), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(_createHandle, _engine.newFunction(Handlers::Class::create), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.setProperty(_extendedHandle, _engine.newFunction(Handlers::Class::extended), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ScriptHandler::createClass(QScriptValue arguments) {
	return createClass(createClassPrototype(), arguments);
}

QScriptValue ScriptHandler::createClass(QScriptValue prototype, QScriptValue arguments) {
	QScriptValue instancePrototype = _engine.newObject();
	instancePrototype.setPrototype(prototype.property(_instanceHandle));
	QScriptValue result = _engine.newObject();
	result.setPrototype(prototype);
	result.setProperty(_instanceHandle, instancePrototype, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	prototype.property(_extendedHandle).call(result, arguments);
	return result;
}

QScriptValue ScriptHandler::createInstancePrototype() {
	QScriptValue global = _engine.globalObject();
	QScriptValue prototype = _engine.newObject();
	prototype.setPrototype(global.property("Object").prototype());
	prototype.setProperty(_constructorHandle, _engine.newFunction(Handlers::Class::constructor), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return prototype;
}

QScriptValue ScriptHandler::createInstance(QScriptValue parent, QScriptValue arguments) {
	QScriptValue prototype = parent.property(_instanceHandle);
	QScriptValue result = _engine.newObject();
	result.setPrototype(prototype);
	prototype.property(_constructorHandle).call(result, arguments);
	return result;
}
