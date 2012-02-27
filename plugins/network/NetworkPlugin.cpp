#include "NetworkPlugin.h"

EXPORT_PLUGIN(NetworkPlugin)

const QString NetworkPlugin::PLUGIN_NAME("network");
const int NetworkPlugin::PLUGIN_VERSION(1);

const QString NetworkPlugin::VARIABLE_NAME("Network");

QString NetworkPlugin::name() const {
	return PLUGIN_NAME;
}

int NetworkPlugin::version() const {
	return PLUGIN_VERSION;
}

void NetworkPlugin::install(ScriptEngineInterface* engine) {
	engine_ = engine;
	QScriptValue object = engine_->newObject();
	object.setProperty("sendToClient", engine->newFunction(NetworkPlugin::sendToClient), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	object.setProperty("sendToServer", engine->newFunction(NetworkPlugin::sendToServer), QScriptValue::ReadOnly | QScriptValue::Undeletable);
	engine_->globalObject().setProperty(VARIABLE_NAME, object, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void NetworkPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
}

QScriptValue NetworkPlugin::sendToClient(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue argument = context->argument(0);
		if (argument.isQObject()) {
			PacketInterface* packet = dynamic_cast<PacketInterface*>(argument.toQObject());
			if (packet) {
				QByteArray data((const char*) packet->data(), packet->length());
				ScriptEngineInterface* scriptEngine = (ScriptEngineInterface*) engine;
				scriptEngine->hook()->sendToClient(data);
				return true;
			}
		}
	}
	return false;
}

QScriptValue NetworkPlugin::sendToServer(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue argument = context->argument(0);
		if (argument.isQObject()) {
			PacketInterface* packet = dynamic_cast<PacketInterface*>(argument.toQObject());
			if (packet) {
				QByteArray data((const char*) packet->data(), packet->length());
				ScriptEngineInterface* scriptEngine = (ScriptEngineInterface*) engine;
				scriptEngine->hook()->sendToServer(data);
				return true;
			}
		}
	}
	return false;
}
