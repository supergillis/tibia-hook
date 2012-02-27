#include "MemoryPlugin.h"

EXPORT_PLUGIN(MemoryPlugin)

const QString MemoryPlugin::PLUGIN_NAME("memory");
const int MemoryPlugin::PLUGIN_VERSION(1);

const QString MemoryPlugin::VARIABLE_NAME("Memory");

QString MemoryPlugin::name() const {
	return PLUGIN_NAME;
}

int MemoryPlugin::version() const {
	return PLUGIN_VERSION;
}

void MemoryPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;
	engine_->globalObject().setProperty(VARIABLE_NAME, engine->newQObject(this), QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void MemoryPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
}

quint8 MemoryPlugin::readU8(quint32 address) {
	quint8 value = *((quint8*) address);
	return value;
}

quint16 MemoryPlugin::readU16(quint32 address) {
	quint16 value = *((quint16*) address);
	return value;
}

quint32 MemoryPlugin::readU32(quint32 address) {
	quint32 value = *((quint32*) address);
	return value;
}

QString MemoryPlugin::readString(quint32 address) {
	return QString((const char*) address);
}
