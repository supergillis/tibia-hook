#include "NetworkModule.h"

const QString NetworkModule::PLUGIN_NAME("network");
const QString NetworkModule::VARIABLE_NAME("Network");

NetworkModule::NetworkModule(QObject* parent) :
		Module(parent), hook_(NULL) {
}

QString NetworkModule::name() const {
	return PLUGIN_NAME;
}

bool NetworkModule::install(ModuleManager* manager) {
	hook_ = manager->hook();
	QScriptEngine* engine = manager->engine();
	QScriptValue networkObject = engine->newQObject(this);
	engine->globalObject().setProperty(VARIABLE_NAME, networkObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

void NetworkModule::sendToServer(Packet* packet) {
	hook_->sendToServer(packet);
}
