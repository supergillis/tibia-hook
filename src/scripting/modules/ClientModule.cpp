#include "ClientModule.h"
#include "ClassModule.h"
#include "Hook.h"
#include "Packet.h"
#include "DecryptedMessage.h"

const QString ClientModule::PLUGIN_NAME("client");
const QString ClientModule::VARIABLE_NAME("Client");

ClientModule::ClientModule(QObject* parent) :
		Module(parent) {
}

QString ClientModule::name() const {
	return PLUGIN_NAME;
}

bool ClientModule::install(ModuleManager* manager) {
	QScriptEngine* engine = manager->engine();
	ClassModule* classModule = (ClassModule*) manager->lookup(ClassModule::PLUGIN_NAME);
	if (classModule) {
		QScriptValue clientObject = classModule->createRootInstance();
		clientObject.setProperty("sendPacket", engine->newFunction(ClientModule::sendPacket));
		clientObject.setProperty("sendKeyPress", engine->newFunction(ClientModule::sendKeyPress));
		engine->globalObject().setProperty(VARIABLE_NAME, clientObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		return true;
	}
	return false;
}

QScriptValue ClientModule::sendPacket(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		Packet* packet = qobject_cast<Packet*>(value.toQObject());
		if (packet) {
			ScriptHandler* handler = qobject_cast<ScriptHandler*>(engine->parent());
			if (handler) {
				DecryptedMessage message(packet);
				handler->hook()->sendToClient(&message);
				return true;
			}
		}
	}
	return context->throwError("invalid call to sendPacket(Packet)");
}

QScriptValue ClientModule::sendKeyPress(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			ScriptHandler* handler = qobject_cast<ScriptHandler*>(engine->parent());
			if (handler) {
				handler->hook()->sendKeyPress(value.toInt32());
				return true;
			}
		}
	}
	return context->throwError("invalid call to sendKeyPress(Number)");
}
