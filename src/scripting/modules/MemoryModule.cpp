#include "MemoryModule.h"

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
	quint8 value = *((quint8*) address);
	return value;
}

quint16 MemoryModule::readU16(quint32 address) {
	quint16 value = *((quint16*) address);
	return value;
}

quint32 MemoryModule::readU32(quint32 address) {
	quint32 value = *((quint32*) address);
	return value;
}

QString MemoryModule::readString(quint32 address) {
	return QString((const char*) address);
}
