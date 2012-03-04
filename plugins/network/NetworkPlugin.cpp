/* Copyright (c) 2012 Gillis Van Ginderachter <supergillis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

void NetworkPlugin::install(HookInterface* hook) {
	hook_ = hook;
	engine_ = hook->engine();
	QScriptValue object = engine_->newObject();
	QScriptValue thisObject = engine_->newQObject(this);
	QScriptValue sendToClient = engine_->newFunction(NetworkPlugin::sendToClient);
	QScriptValue sendToServer = engine_->newFunction(NetworkPlugin::sendToServer);
	sendToClient.setData(thisObject);
	sendToServer.setData(thisObject);
	object.setProperty("sendToClient", sendToClient, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	object.setProperty("sendToServer", sendToServer, QScriptValue::ReadOnly | QScriptValue::Undeletable);
	engine_->globalObject().setProperty(VARIABLE_NAME, object, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void NetworkPlugin::uninstall() {
	engine_->globalObject().setProperty(VARIABLE_NAME, QScriptValue::UndefinedValue);
	engine_ = NULL;
	hook_;
}

QScriptValue NetworkPlugin::sendToClient(QScriptContext* context, QScriptEngine* engine) {
	if (context->argumentCount() == 1) {
		QScriptValue argument = context->argument(0);
		if (argument.isQObject()) {
			PacketInterface* packet = dynamic_cast<PacketInterface*>(argument.toQObject());
			if (packet) {
				QByteArray data((const char*) packet->data(), packet->length());
				NetworkPlugin* plugin = dynamic_cast<NetworkPlugin*>(context->callee().data().toQObject());
				plugin->hook_->sender()->sendToClient(data);
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
				NetworkPlugin* plugin = dynamic_cast<NetworkPlugin*>(context->callee().data().toQObject());
				plugin->hook_->sender()->sendToServer(data);
				return true;
			}
		}
	}
	return false;
}
