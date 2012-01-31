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
	engine->globalObject().setProperty(VARIABLE_NAME, engine->newQObject(this), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

quint8 MemoryModule::readU8(quint32 address) {
	return Memory::readU8(address);
}

quint16 MemoryModule::readU16(quint32 address) {
	return Memory::readU16(address);
}

quint32 MemoryModule::readU32(quint32 address) {
	return Memory::readU32(address);
}

QString MemoryModule::readString(quint32 address) {
	return Memory::readString(address);
}
