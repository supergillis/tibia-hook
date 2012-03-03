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

#include "ScriptReceiver.h"
#include "ReadOnlyPacketProxy.h"

ScriptReceiver::ScriptReceiver(SenderInterface* sender, QObject* parent) :
		ReceiverInterface(), engine_(sender, parent) {
	receiveFromClientHandle_ = engine_.toStringHandle("receiveFromClient");
	receiveFromServerHandle_ = engine_.toStringHandle("receiveFromServer");
}

ScriptReceiver::~ScriptReceiver() {
	foreach(ScriptPluginInterface* plugin, plugins_) {
		qDebug() << "uninstalling" << plugin->name();
		plugin->uninstall();
	}
}

void ScriptReceiver::reload() {
	engine_.reload();
}

void ScriptReceiver::install(ScriptPluginInterface* plugin) {
	qDebug() << "installing" << plugin->name();
	plugin->install(&engine_);
	plugins_.append(plugin);
}

/**
  * This function runs in Tibia's thread.
  */
bool ScriptReceiver::receiveFromClient(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromClientHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}

/**
  * This function runs in Tibia's thread.
  */
bool ScriptReceiver::receiveFromServer(const QByteArray& data) {
	QScriptValue callback = engine_.globalObject().property(receiveFromServerHandle_);
	if (callback.isFunction()) {
		ReadOnlyPacketInterface* packet = new ReadOnlyPacketProxy(data);
		QScriptValue value = engine_.newQObject(packet, QScriptEngine::ScriptOwnership);
		QScriptValueList args;
		QScriptValue result = callback.call(engine_.globalObject(), args << value);
		return result.isBool() ? result.toBool() : true;
	}
	return true;
}