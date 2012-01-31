#include "ClientModule.h"
#include "DecryptedMessage.h"

const QString ClientModule::PLUGIN_NAME("client");
const QString ClientModule::VARIABLE_NAME("Client");

ClientModule::ClientModule(QObject* parent) :
		Module(parent), hook_(NULL) {
}

QString ClientModule::name() const {
	return PLUGIN_NAME;
}

bool ClientModule::install(ModuleManager* manager) {
	hook_ = manager->hook();
	QScriptEngine* engine = manager->engine();
	QScriptValue clientObject = engine->newQObject(this);
	engine->globalObject().setProperty(VARIABLE_NAME, clientObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	return true;
}

void ClientModule::sendPacket(Packet* packet) {
	DecryptedMessage message(packet);
	hook_->sendToClient(&message);
}

void ClientModule::sendKeyPress(int key) {
	hook_->sendKeyPress(key);
}
