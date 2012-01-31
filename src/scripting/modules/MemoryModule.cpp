#include "MemoryModule.h"
#include "ClassModule.h"
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
	ClassModule* classModule = (ClassModule*) manager->lookup(ClassModule::PLUGIN_NAME);
	if (classModule) {
		QScriptValue memoryObject = classModule->createRootInstance();
		memoryObject.setProperty("readU8", engine->newFunction(MemoryModule::readU8));
		memoryObject.setProperty("readU16", engine->newFunction(MemoryModule::readU16));
		memoryObject.setProperty("readU32", engine->newFunction(MemoryModule::readU32));
		memoryObject.setProperty("readString", engine->newFunction(MemoryModule::readString));
		engine->globalObject().setProperty(VARIABLE_NAME, memoryObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		return true;
	}
	return false;
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
