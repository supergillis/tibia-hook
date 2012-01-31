#include "PacketModule.h"
#include "ReadOnlyPacket.h"
#include "ReadWritePacket.h"

const QString PacketModule::PLUGIN_NAME("packet");
const QString PacketModule::VARIABLE_NAME("Packet");

PacketModule::PacketModule(QObject* parent) :
		Module(parent) {
}

QString PacketModule::name() const {
	return PLUGIN_NAME;
}

bool PacketModule::install(ModuleManager* manager) {
	QScriptEngine* engine = manager->engine();
	QScriptValue constructor = engine->newFunction(PacketModule::constructor);
	engine->globalObject().setProperty(VARIABLE_NAME, constructor, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

QScriptValue PacketModule::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->isCalledAsConstructor() && context->argumentCount() == 0) {
		QScriptValue object = context->thisObject();
		return engine->newQObject(object, new ReadWritePacket(), QScriptEngine::ScriptOwnership);
	}
	return context->throwError("invalid call to Packet constructor");
}
