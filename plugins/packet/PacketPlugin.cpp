#include "PacketPlugin.h"

#include <QDebug>

EXPORT_PLUGIN(PacketPlugin)

const QString PacketPlugin::PLUGIN_NAME("packet");
const int PacketPlugin::PLUGIN_VERSION(1);

const QString PacketPlugin::VARIABLE_NAME("Packet");

QString PacketPlugin::name() const {
	return PLUGIN_NAME;
}

int PacketPlugin::version() const {
	return PLUGIN_VERSION;
}

void PacketPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;
	engine_->globalObject().setProperty(VARIABLE_NAME, engine->newFunction(PacketPlugin::constructor), QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void PacketPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
}

QScriptValue PacketPlugin::constructor(QScriptContext* context, QScriptEngine* engine) {
	if (context->isCalledAsConstructor() && context->argumentCount() == 0) {
		QScriptValue object = context->thisObject();
		ScriptEngineInterface* scriptEngine = (ScriptEngineInterface*) engine;
		return engine->newQObject(object, scriptEngine->createReadWritePacket(), QScriptEngine::ScriptOwnership);
	}
	return context->throwError("invalid call to Packet constructor");
}
