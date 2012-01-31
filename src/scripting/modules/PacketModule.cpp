#include "PacketModule.h"
#include "ClassModule.h"
#include "ReadWritePacket.h"

QString PacketModule::name() const {
	return "packet";
}

void PacketModule::install() {
	QScriptEngine* engine = handler()->engine();
	QScriptValue rootClass = engine->globalObject().property("Class");
	if (rootClass.isObject()) {
		QScriptValue packetClass = ClassModule::extendClass(engine, rootClass);
		QScriptValue packetInstance = packetClass.property("instance");
		packetInstance.setProperty("constructor", engine->newFunction(PacketModule::constructor));
		packetInstance.setProperty("readU8", engine->newFunction(PacketModule::readU8));
		packetInstance.setProperty("readU16", engine->newFunction(PacketModule::readU16));
		packetInstance.setProperty("readU32", engine->newFunction(PacketModule::readU32));
		packetInstance.setProperty("readString", engine->newFunction(PacketModule::readString));
		packetInstance.setProperty("writeU8", engine->newFunction(PacketModule::writeU8));
		packetInstance.setProperty("writeU16", engine->newFunction(PacketModule::writeU16));
		packetInstance.setProperty("writeU32", engine->newFunction(PacketModule::writeU32));
		packetInstance.setProperty("writeString", engine->newFunction(PacketModule::writeString));
		engine->globalObject().setProperty("Packet", packetClass, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	}
	else {
		qDebug() << "could not find root class";
	}
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
