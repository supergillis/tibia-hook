#include "NetworkModule.h"
#include "ClassModule.h"
#include "Hook.h"
#include "Packet.h"
#include "DecryptedMessage.h"

QString NetworkModule::name() const {
	return "network";
}

void NetworkModule::install() {
	QScriptEngine* engine = handler()->engine();
	QScriptValue rootClass = engine->globalObject().property("Class");
	if (rootClass.isObject()) {
		QScriptValue networkObject = ClassModule::createInstance(engine, rootClass);
		networkObject.setProperty("sendToServer", engine->newFunction(NetworkModule::sendToServer));
		networkObject.setProperty("sendToClient", engine->newFunction(NetworkModule::sendToClient));
		engine->globalObject().setProperty("Network", networkObject, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	}
	else {
		qDebug() << "could not find root class";
	}
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
