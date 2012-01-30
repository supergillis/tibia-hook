#include "ClientModule.h"
#include "ClassModule.h"
#include "Hook.h"
#include "Packet.h"
#include "DecryptedMessage.h"

QString ClientModule::name() const {
	return "client";
}

void ClientModule::install() {
	QScriptEngine* engine = handler()->scriptEngine();
	QScriptValue rootClass = engine->globalObject().property("Class");
	if (rootClass.isObject()) {
		QScriptValue clientObject = ClassModule::createInstance(engine, rootClass);
		clientObject.setProperty("sendPacket", engine->newFunction(ClientModule::sendPacket));
		clientObject.setProperty("sendKeyPress", engine->newFunction(ClientModule::sendKeyPress));
		engine->globalObject().setProperty("Client", clientObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	}
}

QScriptValue ClientModule::sendPacket(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		Packet* packet = qobject_cast<Packet*>(value.toQObject());
		if (packet) {
			DecryptedMessage message(packet);
			Hook::getInstance()->sendToClient(&message);
			return true;
		}
	}
	return context->throwError("invalid call to sendPacket(Packet)");
}

QScriptValue ClientModule::sendKeyPress(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue value = context->argument(0);
		if (value.isNumber()) {
			Hook::getInstance()->sendKeyPress(value.toInt32());
			return true;
		}
	}
	return context->throwError("invalid call to sendKeyPress(Number)");
}
