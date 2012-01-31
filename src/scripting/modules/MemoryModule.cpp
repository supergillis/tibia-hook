#include "MemoryModule.h"
#include "Memory.h"

const QString MemoryModule::PLUGIN_NAME("memory");
const QString MemoryModule::VARIABLE_NAME("Memory");

MemoryModule::MemoryModule(QObject* parent) :
		Module(parent) {
}

QString MemoryModule::name() const {
	return PLUGIN_NAME;
}

bool MemoryModule::install(ModuleManager* manager) {
	QScriptEngine* engine = manager->engine();
	QScriptValue constructor = engine->newFunction(MemoryModule::constructor);
	constructor.setProperty("readU8", engine->newFunction(MemoryModule::readU8));
	constructor.setProperty("readU16", engine->newFunction(MemoryModule::readU16));
	constructor.setProperty("readU32", engine->newFunction(MemoryModule::readU32));
	constructor.setProperty("readString", engine->newFunction(MemoryModule::readString));
	engine->globalObject().setProperty(VARIABLE_NAME, constructor, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

QScriptValue MemoryModule::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->isCalledAsConstructor() && context->argumentCount() == 1) {
		QScriptValue object = context->thisObject();
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return engine->newQObject(object, new MemoryObject(address.toUInt32()), QScriptEngine::ScriptOwnership);
		}
	}
	return context->throwError("invalid call to Memory(Number) constructor");
}

QScriptValue MemoryModule::readU8(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return Memory::readU8(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readU8(Number)");
}

QScriptValue MemoryModule::readU16(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return Memory::readU16(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readU16(Number)");
}

QScriptValue MemoryModule::readU32(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return Memory::readU32(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readU32(Number)");
}

QScriptValue MemoryModule::readString(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue address = context->argument(0);
		if (address.isNumber()) {
			return Memory::readString(address.toUInt32());
		}
	}
	return context->throwError("invalid call to readString(Number)");
}

quint16 MemoryObject::position() const {
	return position_;
}

void MemoryObject::setPosition(quint16 position) {
	position_ = position;
}

void MemoryObject::skip(quint16 count) {
	position_ += count;
}

quint8 MemoryObject::readU8() {
	quint8 value = Memory::readU8(address_ + position_);
	position_ += 1;
	return value;
}

quint16 MemoryObject::readU16() {
	quint16 value = Memory::readU16(address_ + position_);
	position_ += 2;
	return value;
}

quint32 MemoryObject::readU32() {
	quint32 value = Memory::readU32(address_ + position_);
	position_ += 4;
	return value;
}

QString MemoryObject::readString(quint16 maximumLength) {
	QString value = Memory::readString(address_ + position_);
	position_ += maximumLength;
	return value;
}
