#include "NetworkModule.h"
#include "ClassModule.h"
#include "Hook.h"
#include "Packet.h"
#include "DecryptedMessage.h"

const QString NetworkModule::PLUGIN_NAME("network");
const QString NetworkModule::VARIABLE_NAME("Network");

NetworkModule::NetworkModule(QObject* parent) :
		Module(parent) {
}

QString NetworkModule::name() const {
	return PLUGIN_NAME;
}

bool NetworkModule::install(ModuleManager* manager) {
	QScriptEngine* engine = manager->engine();
	ClassModule* classModule = (ClassModule*) manager->lookup(ClassModule::PLUGIN_NAME);
	if (classModule) {
		QScriptValue networkObject = classModule->createRootInstance();
		networkObject.setProperty("sendToServer", engine->newFunction(NetworkModule::sendToServer));
		networkObject.setProperty("sendToClient", engine->newFunction(NetworkModule::sendToClient));
		engine->globalObject().setProperty(VARIABLE_NAME, networkObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		return true;
	}
	return false;
}

QScriptValue NetworkModule::sendToServer(QScriptContext* context, QScriptEngine*) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		Packet* packet = qobject_cast<Packet*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			//Hook::getInstance()->sendToServer(&message);
			return true;
		}
	}
	return context->throwError("invalid call to sendToServer(Packet)");
}

QScriptValue NetworkModule::sendToClient(QScriptContext* context, QScriptEngine*) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		Packet* packet = qobject_cast<Packet*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			//Hook::getInstance()->sendToClient(&message);
			return true;
		}
	}
	return context->throwError("invalid call to sendToServer(Packet)");
}
