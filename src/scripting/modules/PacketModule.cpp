#include "PacketModule.h"
#include "ReadOnlyPacket.h"
#include "ReadWritePacket.h"

const QString PacketModule::PLUGIN_NAME("packet");
const QString PacketModule::VARIABLE_NAME("Packet");

PacketModule::PacketModule(QObject* parent) :
		Module(parent), classModule_(NULL), engine_(NULL) {
}

QString PacketModule::name() const {
	return PLUGIN_NAME;
}

bool PacketModule::install(ModuleManager* manager) {
	classModule_ = (ClassModule*) manager->lookup(ClassModule::PLUGIN_NAME);
	if (classModule_) {
		engine_ = manager->engine();
		packetClass_ = classModule_->extendRootClass();
		packetClass_.setData(engine_->newQObject(this));

		QScriptValue packetInstance = packetClass_.property("instance");
		packetInstance.setProperty("constructor", engine_->newFunction(PacketModule::constructor));
		packetInstance.setProperty("readU8", engine_->newFunction(PacketModule::readU8));
		packetInstance.setProperty("readU16", engine_->newFunction(PacketModule::readU16));
		packetInstance.setProperty("readU32", engine_->newFunction(PacketModule::readU32));
		packetInstance.setProperty("readString", engine_->newFunction(PacketModule::readString));
		packetInstance.setProperty("writeU8", engine_->newFunction(PacketModule::writeU8));
		packetInstance.setProperty("writeU16", engine_->newFunction(PacketModule::writeU16));
		packetInstance.setProperty("writeU32", engine_->newFunction(PacketModule::writeU32));
		packetInstance.setProperty("writeString", engine_->newFunction(PacketModule::writeString));
		engine_->globalObject().setProperty(VARIABLE_NAME, packetClass_, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		return true;
	}
	return false;
}

QScriptValue PacketModule::createReadOnlyPacket(const DecryptedMessage* message) {
	QScriptValue packet = classModule_->createInstance(packetClass_);
	packet.setData(engine_->newQObject(new ReadOnlyPacket(*message), QScriptEngine::ScriptOwnership));
	return packet;
}

QScriptValue PacketModule::createReadWritePacket() {
	QScriptValue packet = classModule_->createInstance(packetClass_);
	packet.setData(engine_->newQObject(new ReadWritePacket(), QScriptEngine::ScriptOwnership));
	return packet;
}

QScriptValue PacketModule::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		QScriptValue data = engine->newQObject(new ReadWritePacket(), QScriptEngine::ScriptOwnership);
		context->thisObject().setData(data);
		return true;
	}
	return context->throwError("invalid call to Packet constructor");
}

QScriptValue PacketModule::readU8(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readU8();
		}
	}
	return context->throwError("invalid call to readU8()");
}

QScriptValue PacketModule::readU16(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readU16();
		}
	}
	return context->throwError("invalid call to readU16()");
}

QScriptValue PacketModule::readU32(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readU32();
		}
	}
	return context->throwError("invalid call to readU32()");
}

QScriptValue PacketModule::readString(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 0) {
		Packet* packet = qobject_cast<Packet*>(context->thisObject().data().toQObject());
		if (packet) {
			return packet->readString();
		}
	}
	return context->throwError("invalid call to readString()");
}

QScriptValue PacketModule::writeU8(QScriptContext* context, QScriptEngine* engine) {
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

QScriptValue PacketModule::writeU16(QScriptContext* context, QScriptEngine* engine) {
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

QScriptValue PacketModule::writeU32(QScriptContext* context, QScriptEngine* engine) {
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

QScriptValue PacketModule::writeString(QScriptContext* context, QScriptEngine* engine) {
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
